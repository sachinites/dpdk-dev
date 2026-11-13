#include "dp_prog_struct.h"
#include "dp_prog_intf_struct.h"

extern int cprintf(const char *fmt, ...);

static void
cp2dp_process_intf_msg(dp_msg_t *dp_msg)
{
    auto *intf_data = (dp_intf_cp2dp_msg_hdr_t *)dp_msg->data;

    switch (dp_msg->opr_type) {
    case DP_CREATE:
        cprintf("DP: create intf %s port_id=%u type=%s "
                "mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
                intf_data->intf_name, intf_data->port_id,
                dp_intf_type_str(intf_data->iftype),
                intf_data->mac_addr[0], intf_data->mac_addr[1],
                intf_data->mac_addr[2], intf_data->mac_addr[3],
                intf_data->mac_addr[4], intf_data->mac_addr[5]);
        break;
    case DP_DEL:
        cprintf("DP: delete intf %s port_id=%u\n",
                intf_data->intf_name, intf_data->port_id);
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
