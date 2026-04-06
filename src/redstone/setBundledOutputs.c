#include "common.h"

cJSON* thl_redstone_msg_create_setBundledOutputs(char *device_id, thl_dir_t direction, unsigned char levels[16]) {
    cJSON *params = params_create_direction(direction);
    if (!params) return NULL;
    
    int int_levels[16];
    for (int i = 0; i < 16; i++) {
        int_levels[i] = levels[i];
    }
    
    params = params_add(params, cJSON_CreateIntArray(int_levels, 16));
    return thl_invoke_msg_convert(device_id, "setBundledOutputs", params);
}

int thl_redstone_setBundledOutputs(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char levels[16]) {
    return thl_bus_invoke_nodata(bus, thl_redstone_msg_create_setBundledOutputs(device_id, direction, levels));
}
