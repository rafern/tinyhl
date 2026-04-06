#include <string.h>
#include <stdlib.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "core.h"

thl_bus_t thl_bus_open(char *path) {
    // TODO: i wonder if you get a new bus file if you disconnect
    //       a device while the computer is on, and if so, if
    //       some kind of mechanism can be made to pick the bus
    //       automatically, instead of always trying hvc0
    if (path == NULL) path = "/dev/hvc0";

	// XXX: i considered O_DIRECT here, but it didn't work, and im
	//      not sure if its even needed for device files, since
	//      they're probably not cached anyway
    int fd = open(path, O_RDWR | O_SYNC | O_NOATIME);
    
    if (fd != -1) {
        // more or less equivalent to running `stty raw -echo` on the FD
        struct termios termios_p;
        tcgetattr(fd, &termios_p);
        termios_p.c_iflag = 0;
        termios_p.c_oflag = 0; // XXX: technically `stty raw` only unsets OPOST
        termios_p.c_lflag = 0; // XXX: technically `stty raw -echo` only unsets ISIG | ICANON | XCASE | ECHO
        termios_p.c_cc[VMIN] = 1;
        termios_p.c_cc[VTIME] = 0;
        tcsetattr(fd, TCSANOW, &termios_p);
    }
    
    return fd;
}

void thl_bus_close(thl_bus_t bus) {
    close(bus);
}

cJSON* thl_bus_read(thl_bus_t bus) {
    char *out_buf = NULL;
    size_t out_buf_len = 0;
    char accum_buf[1024];
    int found_start = 0;
    int need_flush = 0;

    while (1) {
        // TODO: implement timeouts. it blocks forever atm
    	size_t bytes = read(bus, accum_buf, 1024);
    	if (!bytes) break;
    	
    	char *accum_start = accum_buf;
    	char *accum_end = accum_start + bytes;
    	
        reprocess:
    	if (need_flush) {
    	    // get rid of all input data until end delimiter
    	    char *end_delim = memchr(accum_start, '\0', accum_end - accum_start);
    	    if (end_delim == NULL) continue; // no end delim, keep flushing
    	    need_flush = 0;
    	    accum_start = end_delim + 1;
    	    if (accum_start == accum_end) continue; // end found, but no more data
    	}
    	
    	if (!found_start) {
    	    if (*accum_start == '\0') {
    	        found_start = 1;
    	        accum_start++;
    	    } else {
    	        need_flush = 1;
    	        goto reprocess;
    	    }
    	}
    	
    	if (found_start) {
    	    // TODO (maybe): flush data past end delim if any?
    	    // should not be necessary, but invalid data before
    	    // start delimiter is already being handled, which is
    	    // also unnecessary, so...
    	    char *end_delim = memchr(accum_start, '\0', accum_end - accum_start);
    	    if (end_delim) accum_end = end_delim;
    	    size_t len = accum_end - accum_start;
    	    char* new_out_buf = realloc(out_buf, out_buf_len + len);
    	    if (!new_out_buf) {
    	        free(out_buf);
    	        return NULL;
    	    }
    	    
    	    out_buf = new_out_buf;
    	    memcpy(out_buf + out_buf_len, accum_start, len);
    	    out_buf_len += len;
    	    if (end_delim) break;
    	}
    }
    
    if (out_buf) {
        return cJSON_ParseWithLength(out_buf, out_buf_len);
    }
    
    return NULL;
}

int thl_bus_write(thl_bus_t bus, cJSON *msg) {
    // fast path: try to fit everything in a preallocated 1024
    // byte buffer, with padding. the reason why padding is
    // included is that cJSON states in the documentation that
    // the preallocated buffer function doesn't estimate length
    // properly, which implies that you can get random
    // overflows... yikes!
    // messages are also surrounded by null chars for protocol
    // reasons
    char fast_buf[1024];
    fast_buf[0] = '\0';

    char *fast_offset_buf = fast_buf + 1;
    if (cJSON_PrintPreallocated(msg, fast_offset_buf, 1023 - 5, 0)) {
        size_t out_len = strnlen(fast_offset_buf, 1024 - 2);
        fast_offset_buf[out_len] = '\0';
        size_t write_len = out_len + 2;
        return write(bus, fast_buf, write_len) == write_len;
    } else {
    	// fallback. write message in pieces, and allocate
    	// buffer in heap
    	char *slow_buf = cJSON_Print(msg);
    	if (!slow_buf) return 0;
    	
    	size_t slow_buf_len = strlen(slow_buf);
		
		if (write(bus, fast_buf, 1) != 1) goto fail;
        if (write(bus, slow_buf, slow_buf_len) != slow_buf_len) goto fail;
		if (write(bus, fast_buf, 1) != 1) goto fail;
		
        free(slow_buf);
        return 1;
 
        fail:
        free(slow_buf);
        return 0;
    }
}

cJSON* thl_bus_handle_response(thl_bus_t bus, const char *expected_response_type) {
    cJSON *response = thl_bus_read(bus);
    cJSON *data = NULL;

    if (response) {
        if (cJSON_IsObject(response)) {
            const cJSON* type = cJSON_GetObjectItemCaseSensitive(response, "type");
            if (cJSON_IsString(type) && strcmp(type->valuestring, expected_response_type) == 0) {
                data = cJSON_DetachItemFromObjectCaseSensitive(response, "data");
                if (!data) {
                    // response has no data. fall back to a null json literal
                    data = cJSON_CreateNull();
                }
            }
        }
        
        cJSON_Delete(response);
    }

    return data;
}

cJSON* thl_bus_request(thl_bus_t bus, cJSON *msg, const char *expected_response_type) {
    if (!msg) return NULL;
    int write_ok = thl_bus_write(bus, msg);
    cJSON_Delete(msg);
    if (!write_ok) return NULL;
    return thl_bus_handle_response(bus, expected_response_type);
}

cJSON* thl_bus_invoke(thl_bus_t bus, cJSON *msg) {
    return thl_bus_request(bus, msg, "result");
}

int thl_bus_invoke_nodata(thl_bus_t bus, cJSON *msg) {
    cJSON *res = thl_bus_invoke(bus, msg);
    if (!res) return 0;
    cJSON_Delete(res);
    return 1;
}
