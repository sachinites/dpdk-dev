#ifndef __DP_PROG_STRUCT_H__
#define __DP_PROG_STRUCT_H__

#include <stdint.h>

#define CP2DP_MSG_SIZE_MAX 1024

typedef enum DP_COMPONENT_TYPE_ {

    INTF_TABLE

} DP_COMPONENT_TYPE_T;

typedef enum DP_OPR_TYPE_ {

    DP_CREATE,
    DP_DEL,
    DP_UPDATE,
    DP_READ,
    DP_L3_NORTHBOUND_IN,
    DP_CLEAR
    
} DP_OPR_TYPE_T;

typedef struct dp_msg_ {

    uint8_t data[CP2DP_MSG_SIZE_MAX];
    DP_COMPONENT_TYPE_T component_type;
    DP_OPR_TYPE_T opr_type;
    uint32_t data_size;
    uint16_t flags;
    char padding[2];
    
} dp_msg_t;

#endif 
