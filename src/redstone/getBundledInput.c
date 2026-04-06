#include "common.h"

cJSON* thl_redstone_msg_create_getBundledInput(char *device_id, thl_dir_t direction) {
    return thl_invoke_msg_convert(device_id, "getBundledInput", params_create_direction(direction));
}

int thl_redstone_pinvoke_getBundledInput(thl_bus_t bus, unsigned char inputs[16], thl_pmsg_t *pmsg) {
    return handle_response_getBundledX(thl_bus_pinvoke(bus, pmsg), inputs);
}

int thl_redstone_getBundledInput(thl_bus_t bus, unsigned char inputs[16], char *device_id, thl_dir_t direction) {
    return handle_response_getBundledX(thl_bus_invoke(bus, thl_redstone_msg_create_getBundledInput(device_id, direction)), inputs);
}
