#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>

#define RX_RING_SIZE 1024
#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

static int
port_init(uint16_t port, struct rte_mempool *mbuf_pool)
{
    struct rte_eth_conf port_conf = {0};
    const uint16_t rx_rings = 1;
    int retval;

    retval = rte_eth_dev_configure(port, rx_rings, 0, &port_conf);
    if (retval < 0) return retval;

    retval = rte_eth_rx_queue_setup(port, 0, RX_RING_SIZE,
                                    rte_eth_dev_socket_id(port),
                                    NULL, mbuf_pool);
    if (retval < 0) return retval;

    retval = rte_eth_dev_start(port);
    if (retval < 0) return retval;

    struct rte_ether_addr addr;
    rte_eth_macaddr_get(port, &addr);
    printf("Port %u MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           port,
           addr.addr_bytes[0], addr.addr_bytes[1], addr.addr_bytes[2],
           addr.addr_bytes[3], addr.addr_bytes[4], addr.addr_bytes[5]);

    rte_eth_promiscuous_enable(port);
    return 0;
}

int main(int argc, char *argv[])
{
    int ret = rte_eal_init(argc, argv);
    if (ret < 0) rte_exit(EXIT_FAILURE, "Error with EAL init\n");

    uint16_t port_count = rte_eth_dev_count_avail();
    printf("Available ports = %u\n", port_count);

    if (port_count < 2)
        rte_exit(EXIT_FAILURE, "Need at least 2 ports (ens4 & ens5)\n");

    struct rte_mempool *mbuf_pool =
        rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * port_count,
                                MBUF_CACHE_SIZE, 0,
                                RTE_MBUF_DEFAULT_BUF_SIZE,
                                rte_socket_id());

    if (!mbuf_pool)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    // Initialize all ports
    for (uint16_t port = 0; port < port_count; port++)
        if (port_init(port, mbuf_pool) != 0)
            rte_exit(EXIT_FAILURE, "Cannot init port %"PRIu16"\n", port);

    printf("Starting packet RX loop...\n");

    while (1) {
        for (uint16_t port = 0; port < port_count; port++) {
            struct rte_mbuf *bufs[BURST_SIZE];
            uint16_t nb_rx = rte_eth_rx_burst(port, 0, bufs, BURST_SIZE);

            for (uint16_t i = 0; i < nb_rx; i++) {
                struct rte_mbuf *mbuf = bufs[i];
                struct rte_ether_hdr *eth =
                    rte_pktmbuf_mtod(mbuf, struct rte_ether_hdr *);

                printf("\n[Port %u] ", port);
                printf("Ether: %02X:%02X:%02X:%02X:%02X:%02X -> "
                       "%02X:%02X:%02X:%02X:%02X:%02X | Type 0x%04X\n",
                       eth->src_addr.addr_bytes[0], eth->src_addr.addr_bytes[1],
                       eth->src_addr.addr_bytes[2], eth->src_addr.addr_bytes[3],
                       eth->src_addr.addr_bytes[4], eth->src_addr.addr_bytes[5],
                       eth->dst_addr.addr_bytes[0], eth->dst_addr.addr_bytes[1],
                       eth->dst_addr.addr_bytes[2], eth->dst_addr.addr_bytes[3],
                       eth->dst_addr.addr_bytes[4], eth->dst_addr.addr_bytes[5],
                       rte_be_to_cpu_16(eth->ether_type));

                if (eth->ether_type == rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4)) {
                    struct rte_ipv4_hdr *ip =
                        (struct rte_ipv4_hdr *)(eth + 1);

                    printf("IPv4: %d.%d.%d.%d -> %d.%d.%d.%d | Proto %u\n",
                           (ip->src_addr >> 24) & 0xff,
                           (ip->src_addr >> 16) & 0xff,
                           (ip->src_addr >> 8) & 0xff,
                           ip->src_addr & 0xff,
                           (ip->dst_addr >> 24) & 0xff,
                           (ip->dst_addr >> 16) & 0xff,
                           (ip->dst_addr >> 8) & 0xff,
                           ip->dst_addr & 0xff,
                           ip->next_proto_id);

                    if (ip->next_proto_id == IPPROTO_TCP) {
                        struct rte_tcp_hdr *tcp =
                            (struct rte_tcp_hdr *)(ip + 1);
                        printf("TCP: src=%u dst=%u\n",
                               rte_be_to_cpu_16(tcp->src_port),
                               rte_be_to_cpu_16(tcp->dst_port));
                    }
                    else if (ip->next_proto_id == IPPROTO_UDP) {
                        struct rte_udp_hdr *udp =
                            (struct rte_udp_hdr *)(ip + 1);
                        printf("UDP: src=%u dst=%u\n",
                               rte_be_to_cpu_16(udp->src_port),
                               rte_be_to_cpu_16(udp->dst_port));
                    }
                }

                rte_pktmbuf_free(mbuf);
            }
        }
    }

    return 0;
}
