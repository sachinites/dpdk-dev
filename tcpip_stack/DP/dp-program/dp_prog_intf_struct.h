#ifndef __DP_PROGR_STRUCT_H__
#define __DP_PROGR_STRUCT_H__

#include <cstdint>

#include "../Interface/intf_const.h"

typedef struct dp_intf_cp2dp_msg_ {

    uint8_t  mac_addr[6];
    char intf_name[DP_INTF_NAME];
    uint32_t port_id;  // key
    uint32_t vlan_id;
    uint32_t iftype;
    uint16_t update_code;
    uint16_t _pad;

} dp_intf_cp2dp_msg_hdr_t;


#endif 