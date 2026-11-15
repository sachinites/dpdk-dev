#include <cstdio>
#include <cstdint>

#include "../../MgmtPlane/CLIBuilder/libcli.h"
#include "../../MgmtPlane/CLIBuilder/cmdtlv.h"
#include "../dp_ctx.h"
#include "dp_intf.h"
#include "intf_const.h"

extern int cprintf(const char *format, ...);
extern dp_ctx_t dp_ctx;

#define CMDCODE_SHOW_DP_INT_BRIEF 100

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

static void
format_ip_addr(uint32_t ip, uint8_t mask, char *buf, size_t bufsz)
{
    if (ip == 0 && mask == 0) {
        snprintf(buf, bufsz, "-");
        return;
    }
    snprintf(buf, bufsz, "%u.%u.%u.%u/%u",
             (ip >> 24) & 0xff,
             (ip >> 16) & 0xff,
             (ip >> 8) & 0xff,
             ip & 0xff,
             mask);
}

static int
show_dataplane_interface_brief_handler(int64_t cmdcode,
                                       Stack_t *tlv_stack,
                                       op_mode enable_or_disable)
{
    (void)cmdcode;
    (void)tlv_stack;
    (void)enable_or_disable;

    uint32_t count = 0;
    for (uint32_t i = 0; i < DP_MAX_INTF; i++) {
        if (dp_ctx.intf_table[i])
            count++;
    }

    if (count == 0) {
        cprintf("\nNo dataplane interfaces present.\n");
        return 0;
    }

    cprintf("\n%-16s %-8s %-12s %-18s %-20s %-8s\n",
            "Interface", "Port", "Type", "IP/Mask", "MAC Address", "Status");
    cprintf("%-16s %-8s %-12s %-18s %-20s %-8s\n",
            "----------------", "--------", "------------",
            "------------------", "--------------------", "--------");

    for (uint32_t i = 0; i < DP_MAX_INTF; i++) {
        dp_intf_t *intf = dp_ctx.intf_table[i];
        if (!intf)
            continue;

        char mac_str[32];
        char ip_str[32];
        format_mac_addr(intf->mac_add.mac, mac_str, sizeof(mac_str));
        format_ip_addr(intf->ip_addr, intf->mask, ip_str, sizeof(ip_str));

        cprintf("%-16s %-8u %-12s %-18s %-20s %-8s\n",
                intf->if_name[0] ? intf->if_name : "-",
                intf->port_id,
                dp_intf_type_str(intf->if_type),
                ip_str,
                mac_str,
                intf->is_up ? "up" : "down");
    }

    return 0;
}

void
dp_mount_show_interface_cli(param_t *show_hook)
{
    /* show dataplane ... */
    static param_t dataplane;
    init_param(&dataplane, CMD, "dataplane", 0, 0, INVALID, 0,
               "Show dataplane information");
    libcli_register_param(show_hook, &dataplane);
    {
        /* show dataplane interface ... */
        static param_t interface;
        init_param(&interface, CMD, "interface", 0, 0, INVALID, 0,
                   "Show dataplane interfaces");
        libcli_register_param(&dataplane, &interface);
        {
            /* show dataplane interface brief */
            static param_t brief;
            init_param(&brief, CMD, "brief",
                       show_dataplane_interface_brief_handler,
                       0, INVALID, 0,
                       "Show dataplane interface brief");
            libcli_register_param(&interface, &brief);
            libcli_set_param_cmd_code(&brief, CMDCODE_SHOW_DP_INT_BRIEF);
            /* Route handler to DP event loop */
            libcli_set_user_flag(&brief, 1);
        }
    }
}
