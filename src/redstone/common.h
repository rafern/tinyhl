#ifndef THL_REDSTONE_COMMON_H
#define THL_REDSTONE_COMMON_H

#include "../redstone.h"

static cJSON* params_add(cJSON *params, cJSON *param) {
    if (param) {
        cJSON_AddItemToArray(params, param);
        return params;
    } else {
        cJSON_Delete(params);
        return NULL;
    }
}

static cJSON* params_add_num(cJSON *params, int num) {
    if (!params) return NULL;
    return params_add(params, cJSON_CreateNumber(num));
}

static cJSON* params_create_direction(thl_dir_t direction) {
    char* dirstr = thl_dir_to_str(direction);
    if (!dirstr) return NULL;
    cJSON *params = cJSON_CreateArray();
    if (!params) return NULL;
    return params_add(params, cJSON_CreateString(dirstr));
}

static cJSON* params_create_direction_level(thl_dir_t direction, unsigned char level) {
    return params_add_num(params_create_direction(direction), level);
}

static cJSON* params_create_direction_level_color(thl_dir_t direction, unsigned char level, thl_bundle_color_t color) {
    return params_add_num(params_create_direction_level(direction, level), color);
}

static int handle_response_getRedstoneX(cJSON *res) {
    if (!res) return -1;
    int ret = -1;
    if (cJSON_IsNumber(res)) ret = res->valueint;
    cJSON_Delete(res);
    return ret;
}

static int handle_response_getBundledX(cJSON *res, unsigned char outputs[16]) {
    if (!res) return 0;
    int ret = 0;
    
    if (cJSON_IsArray(res) && cJSON_GetArraySize(res) == 16) {
        cJSON *elem;
        int i = 0;
        
        cJSON_ArrayForEach(elem, res) {
            if (!cJSON_IsNumber(elem)) break;
            outputs[i++] = elem->valueint;
        }
        
        ret = i == 16;
    }
    
    cJSON_Delete(res);
    return ret;
}

#endif
