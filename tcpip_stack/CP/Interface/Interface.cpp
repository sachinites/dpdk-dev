#include "Interface.h"

#include <cstdio>
#include <cstring>
extern int cprintf (const char* format, ...) ;

Interface::Interface(std::string if_name, InterfaceType_t iftype)
    : if_name(std::move(if_name)),
      ifindex(0),
      pkt_recv(0),
      pkt_sent(0),
      xmit_pkt_dropped(0),
      recvd_pkt_dropped(0),
      is_up(false),
      iftype(iftype)
{
}

Interface::~Interface() {
    
}

void
Interface::PrintInterfaceDetails()
{
    cprintf("if_name=%s ifindex=%u iftype=%d is_up=%d\n",
           if_name.c_str(), ifindex, static_cast<int>(iftype), is_up);
}

void
Interface::SetMacAddr(uint8_t (*mac_add)[6])
{
    (void)mac_add;
}

uint8_t *
Interface::GetMacAddr()
{
    return nullptr;
}

PhysicalInterface::PhysicalInterface(std::string ifname, uint8_t (*mac_add)[6])
    : Interface(std::move(ifname), INTF_TYPE_PHY)
{
    if (mac_add) {
        memcpy(this->mac_add, *mac_add, sizeof(this->mac_add));
    } else {
        memset(this->mac_add, 0, sizeof(this->mac_add));
    }
}

PhysicalInterface::~PhysicalInterface() 
{

}

void
PhysicalInterface::SetMacAddr(uint8_t (*mac_add)[6])
{
    if (mac_add) {
        memcpy(this->mac_add, *mac_add, sizeof(this->mac_add));
    }
}

uint8_t *
PhysicalInterface::GetMacAddr()
{
    return mac_add;
}
