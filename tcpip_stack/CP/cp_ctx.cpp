#include <cstring>

#include "cp_ctx.h"
#include "../tcpconst.h"
#include "../libs/EventDispatcher/PubLockLessEvDis.h"

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *dev_name)
{
    strncpy(cp_ctx->dev_name, dev_name, sizeof(cp_ctx->dev_name) -1);
    cp_ctx->dev_name[sizeof(cp_ctx->dev_name) - 1] = '\0';

    cp_ctx->dev_id = 0;

    cp_ctx->intf_by_name = new std::unordered_map<std::string, InterfaceP>();
    cp_ctx->intf_by_ifindex = new std::unordered_map<uint32_t, InterfaceP>();

    cp_ctx->cp_ev_dis = ll_event_dispatcher_create("CP_EV_DIS", 0);
    ll_event_dispatcher_run(cp_ctx->cp_ev_dis);

    /* Should be integer multiple of 32*/
    bitmap_init(&cp_ctx->if_index_bm, MAX_INTF_INDEX + 1);
}