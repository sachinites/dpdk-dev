#ifndef __INTF_CONST_H__
#define __INTF_CONST_H__


#define DP_INTF_NAME    128

/* Sequence must match exactly with control plane Enum */
enum DP_InterfaceType_t {

    DP_INTF_TYPE_PHY,
    DP_INTF_TYPE_VLAN,
    DP_INTF_TYPE_LOOPBACK,
    DP_INTF_TYPE_RMAC,
    DP_INTF_TYPE_VLAN_FLOOD,
    DP_INTF_TYPE_UNKNOWN
};

static inline const char *
dp_intf_type_str (uint32_t iftype) {

    switch (iftype)
    {
    case DP_INTF_TYPE_PHY:
        return "Physical";
    case DP_INTF_TYPE_VLAN:
        return "VLAN";
    case DP_INTF_TYPE_LOOPBACK:
        return "Loopback";
    case DP_INTF_TYPE_RMAC:
        return "RMAC";
    case DP_INTF_TYPE_VLAN_FLOOD:
        return "VLAN Flood";
    case DP_INTF_TYPE_UNKNOWN:
        return "Unknown";
    default:
        return "Invalid";
    }
}

#endif 