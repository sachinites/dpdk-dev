#include <cstdio>
#include <cstdlib>

#include <rte_eal.h>

int main(int argc, char *argv[])
{
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        fprintf(stderr, "EAL initialization failed\n");
        return EXIT_FAILURE;
    }

    printf("DPDK EAL initialized\n");

    rte_eal_cleanup();
    return EXIT_SUCCESS;
}
