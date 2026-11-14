#include <string.h>
#include <assert.h>

#include "dp_ctx.h"
#include "dp_prog_struct.h"
#include "dp_prog_intf_struct.h"

#include "../Interface/dp_intf.h"
#include "../Interface/dp_intf_store.h"


extern int cprintf(const char *fmt, ...);

extern dp_ctx_t dp_ctx;

static void
cp2dp_process_intf_msg(dp_msg_t *dp_msg)
{
    auto *intf_data = (dp_intf_cp2dp_msg_hdr_t *)dp_msg->data;

    switch (dp_msg->opr_type) {
    case DP_CREATE:
    {        
        assert (dp_ctx.intf_table[intf_data->port_id] == 0);
        
        dp_intf_t *intf = dp_create_interface(intf_data->port_id, 
                            intf_data->iftype, 
                            &intf_data->mac_addr, 
                            intf_data->vlan_id);

        strncpy (intf->if_name, intf_data->intf_name, sizeof (intf->if_name));
        dp_ctx.intf_table[intf_data->port_id] = intf;
        cprintf("DP: Successfully created intf %s port_id=%u\n",
                intf_data->intf_name, intf_data->port_id);        
    }
    break;
    case DP_DEL:
    {
        assert (dp_ctx.intf_table[intf_data->port_id]);
        dp_intf_t *intf = dp_ctx.intf_table[intf_data->port_id];
        dp_ctx.intf_table[intf_data->port_id] = 0;
        dp_delete_interface(intf);
    }
    break;
    default:
        cprintf("DP: intf msg opr_type=%d ignored\n", dp_msg->opr_type);
        break;
    }
}

void
cp2dp_process_dp_msg(dp_msg_t *dp_msg)
{
    switch (dp_msg->component_type) {
    case INTF_TABLE:
        cp2dp_process_intf_msg(dp_msg);
        break;
    default:
        cprintf("DP: unknown component_type=%d\n", dp_msg->component_type);
        break;
    }
}
