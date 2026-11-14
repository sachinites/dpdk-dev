
#ifndef __DP_INTF_STORE__
#define __DP_INTF_STORE__

#include <cstdint>
typedef struct dp_intf_ dp_intf_t;

void
dp_delete_interface (dp_intf_t *intf);

dp_intf_t *
dp_create_interface (uint32_t port_id, uint32_t iftype, uint8_t (*mac_addr)[6], uint16_t vlan_id) ;

#endif