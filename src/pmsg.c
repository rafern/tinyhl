#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "core.h"

thl_pmsg_t* thl_pmsg_create(cJSON *msg) {
    char *data = cJSON_PrintUnformatted(msg);
    if (!data) return NULL;
    
    size_t len = strlen(data);
    char* data_pad = realloc(data, len + 2);
    if (!data_pad) {
        free(data);
        return NULL;
    }
    
    memmove(data_pad + 1, data_pad, len + 1);
    data_pad[0] = '\0';
    
    thl_pmsg_t *pmsg = malloc(sizeof(thl_pmsg_t));
    if (!pmsg) {
        free(data_pad);
        return NULL;
    }
    
    pmsg->data = data_pad;
    pmsg->len = len + 2;
    return pmsg;
}

thl_pmsg_t* thl_pmsg_convert(cJSON *msg) {
    if (!msg) return NULL;
    thl_pmsg_t *pmsg = thl_pmsg_create(msg);
    cJSON_Delete(msg);
    return pmsg;
}

void thl_pmsg_free(thl_pmsg_t *pmsg) {
    if (!pmsg) return;
    free(pmsg->data);
    free(pmsg);
}

int thl_bus_pwrite(thl_bus_t bus, const thl_pmsg_t *pmsg) {
    return write(bus, pmsg->data, pmsg->len) == pmsg->len;
}

cJSON* thl_bus_prequest(thl_bus_t bus, const thl_pmsg_t *pmsg, const char *expected_response_type) {
    if (!thl_bus_pwrite(bus, pmsg)) return NULL;
    return thl_bus_handle_response(bus, expected_response_type);
}

cJSON* thl_bus_pinvoke(thl_bus_t bus, thl_pmsg_t *pmsg) {
    return thl_bus_prequest(bus, pmsg, "result");
}

int thl_bus_pinvoke_nodata(thl_bus_t bus, thl_pmsg_t *pmsg) {
    cJSON *response = thl_bus_pinvoke(bus, pmsg);
    if (!response) return 0;
    cJSON_Delete(response);
    return 1;
}
