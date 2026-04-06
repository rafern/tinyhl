#include <stdlib.h>
#include <string.h>

#include "core.h"

static const thl_pmsg_t LIST_BUS_PMSG = {
    .data = "\0{\"type\":\"list\"}",
    .len = 17,
};

thl_dev_list_t thl_bus_list(thl_bus_t bus) {
    if (!bus) return NULL;
    
    thl_dev_list_t list = NULL;
    
	cJSON *res = thl_bus_prequest(bus, &LIST_BUS_PMSG, "list");
	
	if (res) {
	    if (cJSON_IsArray(res)) {
	        size_t expected_elems = cJSON_GetArraySize(res);
	        size_t list_alloc_size = sizeof(thl_dev_info_t *) * (expected_elems + 1);
	        list = malloc(list_alloc_size);
	        if (!list) goto fail;
	        
	        memset(list, 0, list_alloc_size); // XXX: to make it safe to free in fail goto
	        
	        int i = 0;
	        
	        cJSON *elem;
	        cJSON_ArrayForEach(elem, res) {
	            if (!cJSON_IsObject(elem)) continue;
	            
	            cJSON *device_id_json = cJSON_GetObjectItemCaseSensitive(elem, "deviceId");
	            if (!cJSON_IsString(device_id_json)) continue;
	            
	            cJSON *type_names_json = cJSON_GetObjectItemCaseSensitive(elem, "typeNames");
	            if (!cJSON_IsArray(type_names_json)) continue;
	            
	            thl_dev_info_t *info = malloc(sizeof(thl_dev_info_t));
	            if (!info) goto fail;
	            
	            size_t expected_tn_elems = cJSON_GetArraySize(type_names_json);
	            char **type_names = malloc(sizeof(char *) * (expected_tn_elems + 1));
	            if (!type_names) {
	                free(info);
	                goto fail;
	            }
	            
	            int tn_i = 0;
	            
	            cJSON *tn_elem;
	            cJSON_ArrayForEach(tn_elem, type_names_json) {
	                if (!cJSON_IsString(tn_elem)) continue;
	                
	                tn_elem->type |= cJSON_IsReference; // XXX: take ownership of string
	                type_names[tn_i++] = tn_elem->valuestring;
	            }
	            
	            type_names[tn_i] = NULL;
	            device_id_json->type |= cJSON_IsReference; // XXX: take ownership of string
	            info->device_id = device_id_json->valuestring;
	            info->type_names = type_names;
	            
	            list[i++] = info;
	            
	            if (tn_i != expected_tn_elems) {
	                // XXX: deferred shrinking to make code simpler
	                char** new_type_names = realloc(type_names, sizeof(char *) * (tn_i + 1));
	                if (!new_type_names) goto fail;
	                info->type_names = new_type_names;
	            }
	        }
	        
	        if (i != expected_elems) {
	            thl_dev_list_t list_new = realloc(list, sizeof(thl_dev_info_t *) * (i + 1));
	            if (!list_new) goto fail;
	            list = list_new;
	        }
	    }

	    cJSON_Delete(res);
	}
	
	return list;
	
	fail:
	if (res) cJSON_Delete(res);
	if (list) thl_dev_list_free(list);
	return NULL;
}

void thl_dev_list_free(thl_dev_list_t list) {
    if (!list) return;
    
    for (thl_dev_list_t list_next = list; *list_next; list_next++) {
        thl_dev_info_t *info = *list_next;
        
        for (char **type_names = info->type_names; *type_names; type_names++) {
            free(*type_names);
        }
        
        free(info->device_id);
        free(info->type_names);
        free(info);
    }
    
    free(list);
}

char* thl_dev_list_find(thl_dev_list_t list, const char *wanted_type_name) {
    if (!list) return NULL;
    
    for (thl_dev_list_t list_next = list; *list_next; list_next++) {
        thl_dev_info_t *info = *list_next;
        char **type_names_next = info->type_names;
        
        for (; *type_names_next; type_names_next++) {
            if (strcmp(*type_names_next, wanted_type_name) == 0) {
                return info->device_id;
            }
        }
    }
    
    return NULL;
}

char* thl_bus_find(thl_bus_t bus, const char *wanted_type_name) {
    if (!bus) return NULL;
    
    thl_dev_list_t list = thl_bus_list(bus);
    if (!list) return NULL;
    
    char* device_id = NULL;
    char* found = thl_dev_list_find(list, wanted_type_name);
    if (found) device_id = strdup(found);
    
    thl_dev_list_free(list);
    return device_id;
}
