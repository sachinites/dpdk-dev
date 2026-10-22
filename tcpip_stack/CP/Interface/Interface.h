#include <cstdint>
#include <string>
#include "InterfacEnums.h"

class Interface {

    private:

    protected:
 
        Interface(std::string if_name, InterfaceType_t iftype);

    public:
        std::string if_name;
    
        uint32_t ifindex;
        uint32_t pkt_recv;
        uint32_t pkt_sent;
        uint32_t xmit_pkt_dropped;
        uint32_t recvd_pkt_dropped;
        
        bool is_up;
        InterfaceType_t iftype;
       
        virtual ~Interface();
       
        virtual void PrintInterfaceDetails ();
        virtual void SetMacAddr(uint8_t (*mac_add)[6]);
        virtual uint8_t* GetMacAddr( );

} __attribute__((aligned(8)));


/* ************ */
class PhysicalInterface : public Interface {

    private:
        /* L2 Properties */
        uint8_t mac_add[6];

    protected:
    public:

        PhysicalInterface(std::string ifname, uint8_t (*mac_add)[6]);
        virtual ~PhysicalInterface();
        virtual void SetMacAddr(uint8_t (*mac_add)[6]) final;
        virtual uint8_t* GetMacAddr( ) final;

} __attribute__((aligned(8)));
