#include "InterfaceApi.h"
#include "Interface.h"
#include "../cp_ctx.h"
#include "../../dpal/cp2dp.h"
#include "../../libs/BitOp/bitmap.h"

#include <cstdint>
#include <memory>
#include <string>

extern cp_ctx_t cp_ctx;
extern int cprintf(const char *format, ...);

int
create_loopback_interface(const char *if_name)
{
    if (!if_name || if_name[0] == '\0') {
        cprintf("Error: interface name required\n");
        return -1;
    }

    if (!cp_ctx.intf_by_name || !cp_ctx.intf_by_ifindex) {
        cprintf("Error: control plane not initialized\n");
        return -1;
    }

    std::string name(if_name);
    auto it = cp_ctx.intf_by_name->find(name);
    if (it != cp_ctx.intf_by_name->end()) {
        cprintf("Error: interface %s already exists\n", if_name);
        return -1;
    }

    uint16_t idx = bitmap_get_unset_bit(&cp_ctx.if_index_bm);
    if (idx == UINT16_MAX) {
        cprintf("Error: no free ifindex available\n");
        return -1;
    }

    bitmap_set_bit_at(&cp_ctx.if_index_bm, idx);
    
    auto lo = std::make_shared<LoopbackInterface>(name);
    lo->ifindex = idx;

    (*cp_ctx.intf_by_name)[lo->if_name] = lo;
    (*cp_ctx.intf_by_ifindex)[lo->ifindex] = lo;

    cp2dp_interface_create(*lo);
    cprintf("CP: Created loopback interface %s (ifindex=%u)\n",
            lo->if_name.c_str(), lo->ifindex);
    return 0;
}

int
delete_loopback_interface(const char *if_name)
{
    if (!if_name || if_name[0] == '\0') {
        cprintf("Error: interface name required\n");
        return -1;
    }

    if (!cp_ctx.intf_by_name || !cp_ctx.intf_by_ifindex) {
        cprintf("Error: control plane not initialized\n");
        return -1;
    }

    std::string name(if_name);
    auto it = cp_ctx.intf_by_name->find(name);
    if (it == cp_ctx.intf_by_name->end()) {
        cprintf("Error: interface %s not found\n", if_name);
        return -1;
    }

    InterfaceP intf = it->second;
    if (intf->iftype != INTF_TYPE_LOOPBACK) {
        cprintf("Error: %s is not a loopback interface\n", if_name);
        return -1;
    }

    cp2dp_interface_delete(*intf);
    cp_ctx.intf_by_ifindex->erase(intf->ifindex);
    cp_ctx.intf_by_name->erase(it);

    /* ifindex is released in LoopbackInterface destructor when last ref drops */
    cprintf("CP: Deleted loopback interface %s\n", if_name);
    return 0;
}
