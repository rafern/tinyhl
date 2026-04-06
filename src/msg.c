#include "core.h"

cJSON* thl_invoke_msg_convert(char* device_id, char* method_name, cJSON* params) {
    if (!params) return NULL;
    
    cJSON* msg = cJSON_CreateObject();
    if (!msg) goto fail;

    cJSON* type = cJSON_CreateStringReference("invoke");
    if (!type) goto fail;
    if (!cJSON_AddItemToObjectCS(msg, "type", type)) {
        cJSON_Delete(type);
        goto fail;
    }
    
    cJSON* data = cJSON_CreateObject();
    if (!data) goto fail;
    if (!cJSON_AddItemToObjectCS(msg, "data", data)) {
        cJSON_Delete(data);
        goto fail;
    }
    
    cJSON* device_id_json = cJSON_CreateString(device_id);
    if (!device_id_json) goto fail;
    if (!cJSON_AddItemToObjectCS(data, "deviceId", device_id_json)) {
        cJSON_Delete(device_id_json);
        goto fail;
    }
    
    cJSON* method_name_json = cJSON_CreateString(method_name);
    if (!method_name_json) goto fail;
    if (!cJSON_AddItemToObjectCS(data, "name", method_name_json)) {
        cJSON_Delete(method_name_json);
        goto fail;
    }
    
    if (!cJSON_AddItemToObjectCS(data, "parameters", params)) goto fail;
    
    return msg;
    
    fail:
    cJSON_Delete(msg);
    cJSON_Delete(params);
    return NULL;
}

char* thl_dir_to_str(thl_dir_t dir) {
    switch (dir) {
        case THL_RIGHT: return "right";
        case THL_LEFT: return "left";
        case THL_UP: return "up";
        case THL_DOWN: return "down";
        case THL_FRONT: return "front";
        case THL_BACK: return "back";
        default: return NULL;
    }
}
