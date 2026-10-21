#include "dp_ctx.h"

#include <cstring>

void 
init_dp_ctx(dp_ctx_t *dp_ctx, const char *rtr_name)
{
    strncpy(dp_ctx->rtr_name, rtr_name, sizeof(dp_ctx->rtr_name));
}