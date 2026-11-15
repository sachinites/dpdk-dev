#include <cstdint>
#include <assert.h>
#include "libs/stack/stack.h"
#include "MgmtPlane/CLIBuilder/cmdtlv.h"
#include "MgmtPlane/CLIBuilder/CmdTree/CmdTree.h"
#include "CP/cp_ctx.h"
#include "DP/dp_ctx.h"
#include "libs/EventDispatcher/PubLockLessEvDis.h"

extern cp_ctx_t cp_ctx;
extern dp_ctx_t dp_ctx;

typedef struct unified_cli_data_
{
    cmd_callback cbk;
    int64_t cmdcode;
    Stack_t *tlv_stack;
    op_mode enable_or_disable;

} unified_cli_data_t;

/* Deep-copy CLI TLV stack so the cursor can reuse/clear its original while
 * config/show work is in flight on CP/DP event loops. */
static Stack_t *
tlv_stack_deep_copy(Stack_t *src)
{
    Stack_t *dst = get_new_stack();
    tlv_struct_t *orig;
    tlv_struct_t *copy;
    int i;

    if (!dst) return NULL;
    if (!src || isStackEmpty(src)) return dst;

    for (i = 0; i <= src->top; i++) {
        orig = (tlv_struct_t *)src->slot[i];
        if (!orig) continue;
        copy = (tlv_struct_t *)calloc(1, sizeof(tlv_struct_t));
        if (!copy) {
            while ((orig = (tlv_struct_t *)pop(dst))) free(orig);
            free_stack(dst);
            return NULL;
        }
        *copy = *orig;
        push(dst, copy);
    }
    return dst;
}

void unified_cli_data_free(void *uclid_arg)
{
    unified_cli_data_t *uclid = (unified_cli_data_t *)uclid_arg;
    tlv_struct_t *tlv;

    if (!uclid_arg) return;

    /* uclid always owns a deep-copied TLV stack (and its elements). */
    if (uclid->tlv_stack) {
        while ((tlv = (tlv_struct_t *)pop(uclid->tlv_stack))) {
            free(tlv);
        }
        free_stack(uclid->tlv_stack);
    }
    free(uclid);
}

void
cli_wq_cbk(ll_event_dispatcher_t *ev_dis, void *arg, uint32_t size) {

    unified_cli_data_t *uclid = (unified_cli_data_t *)arg;
    uclid->cbk(uclid->cmdcode, uclid->tlv_stack, uclid->enable_or_disable);
}

void
task_invoke_appln_cbk_handler(int64_t cmdcode,
                              cmd_callback cbk,
                              Stack_t *tlv_stack,
                              op_mode enable_or_disable,
                              uint8_t user_flag) {

    bool show_cli;
    bool submit_to_cp;

    ll_event_dispatcher_t *ev_dis;
    unified_cli_data_t *uclid;

    /* Prepare uclid on the heap */
    uclid = (unified_cli_data_t *)calloc (1, sizeof (unified_cli_data_t));
    uclid->cbk = cbk;
    uclid->cmdcode = cmdcode;
    uclid->enable_or_disable = enable_or_disable;
    /* Create a copy of TLV Stack since multiple config commands can be
     * posted all in one shot (CLI cursor reuses its stack). */
    uclid->tlv_stack = tlv_stack_deep_copy(tlv_stack);
    assert(uclid->tlv_stack);

    if (user_flag == 0) {

        /* To be handover to CP EL Thread */
        ev_dis = cp_ctx.cp_ev_dis;
        submit_to_cp = true;
    }
    else {

        /* To be handover to DP EL Thread */
        ev_dis = dp_ctx.dp_ev_dis;
        submit_to_cp = false;
    }

    show_cli = (enable_or_disable == OPERATIONAL) ? true : false;

    if (submit_to_cp && show_cli) {

        /* Submit show CLI to CP EL Thread using Synchronous enqueue API */
        ll_pkt_q_enqueue_sync(ev_dis, cp_ctx.show_cli_wq, (char *)uclid, sizeof (*uclid));
    }
    else if (submit_to_cp && !show_cli) {

        /* Submit config CLI to CP EL Thread using ASynchronous enqueue API */
        ll_pkt_q_enqueue(ev_dis, cp_ctx.config_cli_wq, (char *)uclid, sizeof (*uclid));
    }
    else if (!submit_to_cp && show_cli) {

        /* Submit show CLI to DP EL Thread using Synchronous enqueue API */
        ll_pkt_q_enqueue_sync(ev_dis, dp_ctx.show_cli_wq, (char *)uclid, sizeof (*uclid));
    }
    else if (!submit_to_cp && !show_cli) {

        /* Submit config CLI to DP EL Thread using Synchronous enqueue API */
        assert(0);
    }
    else {
        assert (0);
    }
}