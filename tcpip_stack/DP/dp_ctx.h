#ifndef __DP_CTX_H__
#define __DP_CTX_H__

#include <cstdint>
#include "Interface/intf_const.h"

typedef struct ll_event_dispatcher_ ll_event_dispatcher_t;
typedef struct ll_pkt_q_ ll_pkt_q_t;
typedef struct dp_intf_ dp_intf_t;

typedef struct dp_ctx_ {

     /* Give some Name to this NW Dev*/
     alignas(8) char dev_name[32];

    /* IPV4 Router ID for a Routing device */
    alignas(8) uint32_t dev_id;

    /* DP Mgmt Event Loop (EventDispatcher )*/
    ll_event_dispatcher_t *dp_ev_dis;
    
    /* Work Queues to recv Data from CP */
    ll_pkt_q_t *cp2dp_wq;

    dp_intf_t *intf_table[DP_MAX_INTF];

} dp_ctx_t;

void 
init_dp_ctx(dp_ctx_t *dp_ctx, const char *dev_name);

#endif 