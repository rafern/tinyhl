#include "common.h"

cJSON* thl_redstone_msg_create_setRedstoneOutput(char *device_id, thl_dir_t direction, unsigned char level) {
    return thl_invoke_msg_convert(device_id, "setRedstoneOutput", params_create_direction_level(direction, level));
}

int thl_redstone_setRedstoneOutput(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char level) {
    return thl_bus_invoke_nodata(bus, thl_redstone_msg_create_setRedstoneOutput(device_id, direction, level));
}
