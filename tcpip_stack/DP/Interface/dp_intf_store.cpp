#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../dp_ctx.h"
#include "dp_intf_store.h"
#include "dp_intf.h"

extern dp_ctx_t dp_ctx;

extern int cprintf (const char* format, ...) ;

dp_intf_t *
dp_create_interface (uint32_t port_id, uint32_t iftype, 
                     uint8_t (*mac_addr)[6], 
                     uint16_t vlan_id) {

    (void)vlan_id; // not used

    dp_intf_t *intf = (dp_intf_t *)calloc(1, sizeof(dp_intf_t));
    intf->port_id = port_id;
    intf->if_type = (DP_InterfaceType_t)iftype;
    intf->if_name[0] = '\0';
    intf->ip_addr = 0;
    intf->mask = 0;
    if (mac_addr) memcpy(&intf->mac_add, (void *)mac_addr, 6);
    intf->is_up = false;
    return intf;
}

static void 
dp_check_and_free_interface (dp_intf_t *intf) {
    
    /* ToDo : assert if interface is holding some resource */

    cprintf ("DP : intf %s deleted\n", intf->if_name);
    free(intf);
}

void
dp_delete_interface (dp_intf_t *intf) {

    assert(intf);
    assert(intf->port_id < DP_MAX_INTF);
    
    /* This fn assumes that interface is already removed from 
        intf_table array */
    assert(dp_ctx.intf_table[intf->port_id] == NULL);
    dp_check_and_free_interface (intf);
}