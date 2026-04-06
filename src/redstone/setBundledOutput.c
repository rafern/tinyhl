#include "common.h"

cJSON* thl_redstone_msg_create_setBundledOutput(char *device_id, thl_dir_t direction, unsigned char level, thl_bundle_color_t color) {
    return thl_invoke_msg_convert(device_id, "setBundledOutput", params_create_direction_level_color(direction, level, color));
}

int thl_redstone_setBundledOutput(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char level, thl_bundle_color_t color) {
    return thl_bus_invoke_nodata(bus, thl_redstone_msg_create_setBundledOutput(device_id, direction, level, color));
}
