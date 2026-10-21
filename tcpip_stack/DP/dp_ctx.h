#ifndef __DP_CTX_H__
#define __DP_CTX_H__

#include <cstdint>

typedef struct dp_ctx_ {

     /* Give some Name to this NW Dev*/
     alignas(8) char rtr_name[32];

    /* IPV4 Router ID for a Routing device */
    alignas(8) uint32_t rtr_id;

} dp_ctx_t;

void 
init_dp_ctx(dp_ctx_t *dp_ctx, const char *rtr_name);

#endif 