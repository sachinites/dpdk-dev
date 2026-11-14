#ifndef __DP_INTF__
#define __DP_INTF__

#include <cstdint>
#include "intf_const.h"
#include "../../libs/common/cmn_struct.h"

typedef struct dp_intf_ {

    alignas(8) char if_name[DP_INTF_NAME];
    alignas(8) DP_InterfaceType_t if_type;
    alignas(8) uint32_t port_id;
    alignas(8) uint32_t ip_addr;
    alignas(8) uint8_t mask;
    alignas(8) bool is_up;
    alignas(8) mac_addr_t mac_add;

} __attribute__((aligned(8))) dp_intf_t;;


#endif 