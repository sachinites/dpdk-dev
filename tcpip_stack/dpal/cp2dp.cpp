#include <memory.h>

#include "cp2dp.h"

/* Libraries*/
#include "../libs/EventDispatcher/PubLockLessEvDis.h"

extern int cprintf(const char *fmt, ...);

/* Control plane Hdr files */
#include "../CP/Interface/Interface.h"

/* Dplane Hdr files (DP/dp-program/*.h )*/

#include "../DP/dp_ctx.h"
#include "../DP/dp-program/dp_prog_struct.h"
#include "../DP/dp-program/dp_prog_intf_struct.h"

extern dp_ctx_t dp_ctx;

void 
cp2dp_submit(dp_msg_t *dp_msg) {

    ll_pkt_q_enqueue(dp_ctx.dp_ev_dis, 
                     dp_ctx.cp2dp_wq, 
                     (char *)dp_msg, 
                     sizeof(dp_msg_t));
}


void
cp2dp_interface_create(Interface& intf) {

    dp_msg_t *dp_msg = (dp_msg_t *)calloc(1, sizeof(dp_msg_t));

        dp_intf_cp2dp_msg_hdr_t *intf_data = 
            (dp_intf_cp2dp_msg_hdr_t *)dp_msg->data;

    dp_msg->component_type = INTF_TABLE;
    dp_msg->opr_type = DP_CREATE;
    dp_msg->data_size = sizeof(dp_intf_cp2dp_msg_hdr_t);
    dp_msg->flags = 0;

    /* Copy the ifindex MAC Address. Only Physical interface has a MAC address */
    if (intf.iftype == INTF_TYPE_PHY) {
        memcpy(intf_data->mac_addr, intf.GetMacAddr(), 6);
    }

    /* Copy interface name*/
    strncpy(intf_data->intf_name, intf.if_name.c_str(), DP_INTF_NAME - 1);

    intf_data->port_id = intf.ifindex;
    intf_data->vlan_id = 0;
    intf_data->iftype = (uint32_t)intf.iftype;
    intf_data->update_code = 0;

    cp2dp_submit(dp_msg);
}

void
cp2dp_interface_delete(Interface& intf) {

    dp_msg_t *dp_msg = (dp_msg_t *)calloc(1, sizeof(dp_msg_t));

        dp_intf_cp2dp_msg_hdr_t *intf_data = 
            (dp_intf_cp2dp_msg_hdr_t *)dp_msg->data;

    dp_msg->component_type = INTF_TABLE;
    dp_msg->opr_type = DP_DEL;
    dp_msg->data_size = sizeof(dp_intf_cp2dp_msg_hdr_t);
    dp_msg->flags = 0;

    /* Copy the ifindex MAC Address. Only Physical interface has a MAC address */
    if (intf.iftype == INTF_TYPE_PHY) {
        memcpy(intf_data->mac_addr, intf.GetMacAddr(), 6);
    }

    /* Copy interface name*/
    strncpy(intf_data->intf_name, intf.if_name.c_str(), DP_INTF_NAME - 1);

    intf_data->port_id = intf.ifindex;
    intf_data->vlan_id = 0;
    intf_data->iftype = (uint32_t)intf.iftype;
    intf_data->update_code = 0;

    cp2dp_submit(dp_msg);
}