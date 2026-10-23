#include "../../MgmtPlane/CLIBuilder/libcli.h"
#include "../../MgmtPlane/CLIBuilder/cmdtlv.h"
#include "../cp_ctx.h"
#include "Interface.h"

#include <algorithm>
#include <cstdio>
#include <vector>

extern int cprintf (const char* format, ...) ;
extern cp_ctx_t cp_ctx;

#define CMDCODE_SHOW_INT_BRIEF 1

static void
format_mac_addr(const uint8_t *mac, char *buf, size_t bufsz)
{
    if (!mac) {
        snprintf(buf, bufsz, "N/A");
        return;
    }
    snprintf(buf, bufsz, "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

static int
show_interface_brief_handler(int64_t cmdcode,
                    Stack_t *tlv_stack,
                    op_mode enable_or_disable){

    (void)cmdcode;
    (void)tlv_stack;
    (void)enable_or_disable;

    if (!cp_ctx.intf_by_name || cp_ctx.intf_by_name->empty()) {
        cprintf("\nNo interfaces configured.\n");
        return 0;
    }

    std::vector<InterfaceP> intfs;
    for (const auto &entry : *cp_ctx.intf_by_name) {
        intfs.push_back(entry.second);
    }
    std::sort(intfs.begin(), intfs.end(),
              [](const InterfaceP &a, const InterfaceP &b) {
                  return a->if_name < b->if_name;
              });

    cprintf("\n%-16s %-20s %-8s\n",
            "Interface", "MAC Address", "Status");
    cprintf("%-16s %-20s %-8s\n",
            "----------------", "--------------------", "--------");

    for (const auto &intf : intfs) {
        char mac_str[32];
        format_mac_addr(intf->GetMacAddr(), mac_str, sizeof(mac_str));
        cprintf("%-16s %-20s %-8s\n",
                intf->if_name.c_str(),
                mac_str,
                intf->is_up ? "up" : "down");
    }

    return 0;
}

void 
Mount_interface_show_cli(param_t *show) {

    static param_t interface;
    init_param(&interface, CMD, "interface", 0, 0, INVALID, 0, "Show Interface..");
    libcli_register_param(show, &interface);
    {
        /* show interface brief*/
        static param_t brief;
        init_param(&brief, CMD, "brief", show_interface_brief_handler, 0, INVALID, 0, "Show Interface brief");
        libcli_register_param(&interface, &brief);
        libcli_set_param_cmd_code(&brief, CMDCODE_SHOW_INT_BRIEF);
    }
}

