#include "cp_ctx.h"

#include <cstring>

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *rtr_name)
{
    strncpy(cp_ctx->rtr_name, rtr_name, sizeof(cp_ctx->rtr_name));
}