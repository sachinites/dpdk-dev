#include "../MgmtPlane/CLIBuilder/libcli.h"
#include "../MgmtPlane/CLIBuilder/cmdtlv.h"

/* extern backend handlers here */
extern void Mount_interface_show_cli(param_t *mount_point);
extern void Mount_interface_config_cli(param_t *mount_point);
extern void DP_Mount_show_CLI(param_t *mount_point);

void nw_init_cli() {

    param_t *show   = libcli_get_show_hook();
    param_t *debug  = libcli_get_debug_hook();
    param_t *config = libcli_get_config_hook();
    param_t *run    = libcli_get_run_hook();
    param_t *clear  = libcli_get_clear_hook();

    /* Implement your CLIs here */
    Mount_interface_show_cli(show);
    Mount_interface_config_cli(config);

    /* DP CLIs */
    DP_Mount_show_CLI(show);
}