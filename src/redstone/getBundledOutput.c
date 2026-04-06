#include "common.h"

cJSON* thl_redstone_msg_create_getBundledOutput(char *device_id, thl_dir_t direction) {
    return thl_invoke_msg_convert(device_id, "getBundledOutput", params_create_direction(direction));
}

int thl_redstone_pinvoke_getBundledOutput(thl_bus_t bus, unsigned char outputs[16], thl_pmsg_t *pmsg) {
    return handle_response_getBundledX(thl_bus_pinvoke(bus, pmsg), outputs);
}

int thl_redstone_getBundledOutput(thl_bus_t bus, unsigned char outputs[16], char *device_id, thl_dir_t direction) {
    return handle_response_getBundledX(thl_bus_invoke(bus, thl_redstone_msg_create_getBundledOutput(device_id, direction)), outputs);
}
