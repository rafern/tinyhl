#include <stdio.h>

#include "../core.h"

int main() {
    thl_bus_t bus = thl_bus_open(NULL);
    if (bus == -1) return 1;

	thl_dev_list_t devs = thl_bus_list(bus);
	if (!devs) {
	    thl_bus_close(bus);
	    return 1;
	}
    
    thl_dev_list_t next_dev = devs;
    int need_space = 0;
    while (*next_dev) {
        thl_dev_info_t *info = *next_dev;
        if (need_space) putchar('\n');
        printf("type names of device \"%s\":\n", info->device_id);
        
        char **next_type_name = info->type_names;
        while (*next_type_name) {
            printf("  - \"%s\"\n", *next_type_name);
            next_type_name++;
        }
        
        next_dev++;
        need_space = 1;
    }
    
    thl_dev_list_free(devs);
	thl_bus_close(bus);
	return 0;
}
