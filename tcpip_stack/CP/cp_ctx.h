#ifndef __CP_H__
#define __CP_H__

#include <cstdint>
#include <unordered_map>
#include <string>
#include "Interface/InterfaceFwd.h"

typedef struct cp_ctx_ {

    /* Give some Name to this NW Dev*/
    alignas(8) char dev_name[32];

    /* IPV4 Router ID for a Routing device */
    alignas(8) uint32_t dev_id;
    
    /* Interface Store , name as a key */
    std::unordered_map<std::string, InterfaceP> *intf_by_name;

     /* Interface Store , ifindex as a key */
    std::unordered_map<uint32_t, InterfaceP> *intf_by_ifindex;    

} cp_ctx_t;

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *dev_name);

#endif 