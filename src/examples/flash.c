#include <stdio.h>
#include <stdlib.h>

#include "../redstone.h"

int main() {
    thl_bus_t bus = thl_bus_open(NULL);
	char* device_id = thl_bus_find(bus, "redstone");
	if (!device_id) return 1;
	
	int ret = 0;
    thl_pmsg_t *on_pmsg = thl_pmsg_convert(thl_redstone_msg_create_setBundledOutput(device_id, THL_UP, 255, 0));
    thl_pmsg_t *off_pmsg = thl_pmsg_convert(thl_redstone_msg_create_setBundledOutput(device_id, THL_UP, 0, 0));
    
    free(device_id);
    
    if (!on_pmsg || !off_pmsg) {
        ret = 2;
        goto exit;
    }
	
	for (int i = 0; i < 100; i++) {
	    thl_bus_pinvoke_nodata(bus, i % 2 == 0 ? on_pmsg : off_pmsg);
	}
    
    exit:
    thl_pmsg_free(on_pmsg);
    thl_pmsg_free(off_pmsg);
	thl_bus_close(bus);
	return 0;
}
