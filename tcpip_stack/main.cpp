#include <cstdio>
#include <cstdlib>

#include <rte_eal.h>
#include "MgmtPlane/CLIBuilder/libcli.h"

#include "DP/dp_ctx.h"
#include "CP/cp_ctx.h"

extern void DPDK_LoadInterfaces();

/* A Nw device has one instance of CP and DP */
cp_ctx_t cp_ctx;
dp_ctx_t dp_ctx;
extern int cprintf (const char* format, ...) ;
extern void std_lib_init (int (*)(const char *format, ...)) ;
extern void nw_init_cli();

int main(int argc, char *argv[])
{
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        fprintf(stderr, "EAL initialization failed\n");
        return EXIT_FAILURE;
    }

    cprintf("DPDK EAL initialized\n");

    libcli_init ();
    std_lib_init(cprintf);

    nw_init_cli();

    /* Data plane must be initialized before control plane because
        control plane during initialization may install certain 
        entries in the dataplane, so data plane should be ready first*/
    init_dp_ctx(&dp_ctx, "DP");
    init_cp_ctx(&cp_ctx, "CP");

    /* Let DPDK Scan the NIC of the system and create Control plane
        and Data plane representation of the interfaces */
    DPDK_LoadInterfaces();


    libcli_init_done ();
    cli_start_shell(); 
    /* Dead Code */

    rte_eal_cleanup();
    return EXIT_SUCCESS;
}
