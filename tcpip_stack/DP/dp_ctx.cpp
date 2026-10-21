#include "dp_ctx.h"

#include <cstring>

void 
init_dp_ctx(dp_ctx_t *dp_ctx, const char *dev_name)
{
    strncpy(dp_ctx->dev_name, dev_name, sizeof(dp_ctx->dev_name));
}