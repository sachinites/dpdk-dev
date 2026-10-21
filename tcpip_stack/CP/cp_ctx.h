#ifndef __CP_H__
#define __CP_H__

#include <cstdint>

typedef struct cp_ctx_ {

    /* Give some Name to this NW Dev*/
    alignas(8) char dev_name[32];

    /* IPV4 Router ID for a Routing device */
    alignas(8) uint32_t dev_id;

} cp_ctx_t;

void 
init_cp_ctx(cp_ctx_t *cp_ctx, const char *dev_name);

#endif 