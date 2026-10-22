#include <rte_ethdev.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

void 
DPDK_LoadInterfaces() {

    uint16_t port_numa_node;
    struct rte_ether_addr mac_addr;
    struct rte_eth_dev_info dev_info;
    char name[RTE_ETH_NAME_MAX_LEN];
    char user_if_name[RTE_ETH_NAME_MAX_LEN];

    uint16_t port_count = rte_eth_dev_count_avail();

    /* DPDK internally recognizes the NICs and assigns them a port_id 
        starting from 0 to port_count - 1. */
    for (uint16_t port_id = 0; port_id < port_count; port_id++) {

        memset(&dev_info, 0, sizeof(dev_info));
        memset(name, 0, sizeof(name));
        memset(&mac_addr, 0, sizeof(mac_addr));
        memset(user_if_name, 0, sizeof(user_if_name));

        rte_eth_dev_info_get(port_id, &dev_info);
        rte_eth_macaddr_get(port_id, &mac_addr);
        rte_eth_dev_get_name_by_port(port_id, name);

        /* rte_eth_dev_socket_id() returns -1 (SOCKET_ID_ANY) when
        NUMA info is unavailable (e.g. single-socket machines).
        Fall back to NUMA node 0 in that case. */
        int socket_id = rte_eth_dev_socket_id(port_id);
        port_numa_node = socket_id < 0 ? 0 : socket_id;

        /* Let us create a control plane view of the ports and install 
            them to data path as well */

        /* DPDK Dettach interfaces from linux, so ifnames like ens2 etc dont
            exist anymore. We have to cook our own interface names */
        snprintf (user_if_name, sizeof (user_if_name), "eth%u", port_id);

        printf("port_id = %u, if_name = %s, pci_bdf = %s, driver = %s, "
                "mac = %02x:%02x:%02x:%02x:%02x:%02x, "
                "max_rx_q = %u, max_tx_q = %u, NUMA node = %u\n",
                port_id, user_if_name, name, dev_info.driver_name,
                mac_addr.addr_bytes[0], mac_addr.addr_bytes[1],
                mac_addr.addr_bytes[2], mac_addr.addr_bytes[3],
                mac_addr.addr_bytes[4], mac_addr.addr_bytes[5],
                dev_info.max_rx_queues, dev_info.max_tx_queues,
                port_numa_node);
    }
}

