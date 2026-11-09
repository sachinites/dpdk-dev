#include <stdint.h>

#include "dp_ctx.h"
#include "../libs/EventDispatcher/PubLockLessEvDis.h"

#include <cstring>

extern int cprintf(const char *fmt, ...);

static void 
cp2dp_wq_cbk(ll_event_dispatcher_t *ev_dis, void *arg, uint32_t size) {


    cprintf ("%s: Called ....\n");
}


void 
init_dp_ctx(dp_ctx_t *dp_ctx, const char *dev_name)
{
    strncpy(dp_ctx->dev_name, dev_name, sizeof(dp_ctx->dev_name) -1);
    dp_ctx->dev_name[sizeof(dp_ctx->dev_name) - 1] = '\0';
    dp_ctx->dev_id = 0;
    dp_ctx->dp_ev_dis = ll_event_dispatcher_create("DP_EV_DIS", 0);
    dp_ctx->cp2dp_wq = ll_pkt_q_create(dp_ctx->dp_ev_dis, 
                            cp2dp_wq_cbk,
                            LL_TASK_PRIORITY_CP_TO_DP, 
                            NULL);

    ll_event_dispatcher_run(dp_ctx->dp_ev_dis);
}