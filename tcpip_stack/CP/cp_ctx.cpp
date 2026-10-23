#include "cp_ctx.h"

#include <cstring>

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *dev_name)
{
    strncpy(cp_ctx->dev_name, dev_name, sizeof(cp_ctx->dev_name) -1);
    cp_ctx->dev_name[sizeof(cp_ctx->dev_name) - 1] = '\0';
    cp_ctx->dev_id = 0;
    cp_ctx->intf_by_name = new std::unordered_map<std::string, InterfaceP>();
    cp_ctx->intf_by_ifindex = new std::unordered_map<uint32_t, InterfaceP>();
}