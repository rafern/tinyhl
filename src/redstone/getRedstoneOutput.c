#include "common.h"

cJSON* thl_redstone_msg_create_getRedstoneOutput(char *device_id, thl_dir_t direction) {
    return thl_invoke_msg_convert(device_id, "getRedstoneOutput", params_create_direction(direction));
}

int thl_redstone_pinvoke_getRedstoneOutput(thl_bus_t bus, thl_pmsg_t *pmsg) {
    return handle_response_getRedstoneX(thl_bus_pinvoke(bus, pmsg));
}

int thl_redstone_getRedstoneOutput(thl_bus_t bus, char *device_id, thl_dir_t direction) {
    return handle_response_getRedstoneX(thl_bus_invoke(bus, thl_redstone_msg_create_getRedstoneOutput(device_id, direction)));
}
