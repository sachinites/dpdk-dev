#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "dp_ctx.h"
#include "../libs/EventDispatcher/PubLockLessEvDis.h"
#include "dp-program/dp_prog_struct.h"
#include "dp-program/dp_prog_intf_struct.h"
#include "Interface/intf_const.h"

extern int cprintf(const char *fmt, ...);

extern void
cp2dp_process_dp_msg(dp_msg_t *dp_msg);

static void
cp2dp_wq_cbk(ll_event_dispatcher_t *ev_dis, void *arg, uint32_t size)
{
    (void)size;
    (void)ev_dis;
    cp2dp_process_dp_msg((dp_msg_t *)arg);
    free (arg);
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