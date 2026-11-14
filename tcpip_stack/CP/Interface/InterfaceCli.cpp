#include "../../MgmtPlane/CLIBuilder/libcli.h"
#include "../../MgmtPlane/CLIBuilder/cmdtlv.h"
#include "../cp_ctx.h"
#include "Interface.h"
#include "InterfaceApi.h"

#include <algorithm>
#include <cstdio>
#include <vector>

extern int cprintf (const char* format, ...) ;
extern cp_ctx_t cp_ctx;

#define CMDCODE_SHOW_INT_BRIEF 1
#define CMDCODE_CONFIG_INTF_LOOPBACK 2

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

static int
config_interface_loopback_handler(int64_t cmdcode,
                                  Stack_t *tlv_stack,
                                  op_mode enable_or_disable)
{
    (void)cmdcode;

    const char *if_name = nullptr;
    tlv_struct_t *tlv = nullptr;

    TLV_LOOP_STACK_BEGIN(tlv_stack, tlv) {
        if (parser_match_leaf_id(tlv->leaf_id, "if-name")) {
            if_name = (const char *)tlv->value;
        }
    } TLV_LOOP_END;

    if (!if_name || if_name[0] == '\0') {
        cprintf("Error: interface name required\n");
        return -1;
    }

    if (enable_or_disable == CONFIG_ENABLE) {
        return create_loopback_interface(if_name);
    }

    if (enable_or_disable == CONFIG_DISABLE) {
        return delete_loopback_interface(if_name);
    }

    return 0;
}

void
Mount_interface_config_cli(param_t *config)
{
    /* config interface ... */
    static param_t interface;
    init_param(&interface, CMD, "interface", 0, 0, INVALID, 0,
               "Configure Interface");
    libcli_register_param(config, &interface);
    {
        /* config interface loopback ... */
        static param_t loopback;
        init_param(&loopback, CMD, "loopback", 0, 0, INVALID, 0,
                   "Configure Loopback Interface");
        libcli_register_param(&interface, &loopback);
        {
            /* config interface loopback <if-name> */
            static param_t if_name;
            init_param(&if_name, LEAF, 0, config_interface_loopback_handler, 0,
                       STRING, "if-name", "Loopback interface name");
            libcli_register_param(&loopback, &if_name);
            libcli_set_param_cmd_code(&if_name, CMDCODE_CONFIG_INTF_LOOPBACK);
        }
    }
}

