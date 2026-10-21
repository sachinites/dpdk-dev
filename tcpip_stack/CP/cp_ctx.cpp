#include "cp_ctx.h"

#include <cstring>

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *dev_name)
{
    strncpy(cp_ctx->dev_name, dev_name, sizeof(cp_ctx->dev_name) -1);
}