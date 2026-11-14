#ifndef __DP_INTF__
#define __DP_INTF__

#include <cstdint>
#include "intf_const.h"
#include "../../libs/common/cmn_struct.h"

typedef struct dp_intf_ {
 
    char if_name[DP_INTF_NAME];
    DP_InterfaceType_t if_type;
    uint32_t port_id; 
    uint32_t ip_addr;
    uint8_t mask;
    bool is_up;
    mac_addr_t mac_add;

} dp_intf_t;


#endif 