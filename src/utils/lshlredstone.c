#include <stdio.h>
#include <string.h>

#include "../redstone.h"

void print_signals(int signal, int bundle_ret, unsigned char bundle_signal[16]) {
    fputs("normal=", stdout);
    if (signal < 0) {
        fputs("<fail>", stdout);
    } else {
        printf("%i", signal);
    }
    
    fputs(", bundle=", stdout);
    if (bundle_ret) {
        putchar('[');
        
        int comma = 0;
        for (int i = 0; i < 16; i++) {
            if (comma) putchar(',');
            printf("%i", bundle_signal[i]);
            comma = 1;
        }
        
        putchar(']');
    } else {
        fputs("<fail>", stdout);
    }
    
    putchar('\n');
}

void print_redstone_dev(thl_bus_t bus, char* device_id) {
    printf("%s:\n", device_id);
    
    for (int dir = 0; dir < 6; dir++) {
        unsigned char bbuf[16];
        
        printf(" - %s:\n   - in: ", thl_dir_to_str(dir));
        int in = thl_redstone_getRedstoneInput(bus, device_id, dir);
        int in_bundle = thl_redstone_getBundledInput(bus, bbuf, device_id, dir);
        print_signals(in, in_bundle, bbuf);
        
        fputs("   - out: ", stdout);
        int out = thl_redstone_getRedstoneOutput(bus, device_id, dir);
        int out_bundle = thl_redstone_getBundledOutput(bus, bbuf, device_id, dir);
        print_signals(out, out_bundle, bbuf);
    }
}

int main() {
    thl_bus_t bus = thl_bus_open(NULL);
    if (bus == -1) return 1;

	thl_dev_list_t devs = thl_bus_list(bus);
	if (!devs) {
	    thl_bus_close(bus);
	    return 1;
	}
    
    thl_dev_list_t next_dev = devs;
    int need_space = 0;
    while (*next_dev) {
        thl_dev_info_t *info = *next_dev;
        char **next_type_name = info->type_names;
        while (*next_type_name) {
            if (strcmp(*next_type_name, "redstone") == 0) {
		        if (need_space) putchar('\n');
		        print_redstone_dev(bus, info->device_id);
	            need_space = 1;
		        break;
		    }
		    
            next_type_name++;
        }
        
        next_dev++;
    }
    
    thl_dev_list_free(devs);
	thl_bus_close(bus);
	return 0;
}
