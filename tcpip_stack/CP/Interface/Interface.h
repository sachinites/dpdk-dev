#include <cstdint>
#include <string>
#include "InterfacEnums.h"

class Interface {

    private:

    protected:
 
        Interface(std::string if_name, InterfaceType_t iftype);

    public:
        alignas(8) std::string if_name;
        alignas(8) uint32_t ifindex;
        alignas(8) InterfaceType_t iftype;
        alignas(8) bool is_up;
       
        virtual ~Interface();
       
        virtual void PrintInterfaceDetails ();
        virtual void SetMacAddr(uint8_t (*mac_add)[6]);
        virtual uint8_t* GetMacAddr( );

} __attribute__((aligned(8)));


/* ************ */
class PhysicalInterface : public Interface {

    private:
        /* L2 Properties */
        alignas(8) uint8_t mac_add[6];

    protected:
    public:

        PhysicalInterface(std::string ifname, uint8_t (*mac_add)[6]);
        virtual ~PhysicalInterface();
        virtual void SetMacAddr(uint8_t (*mac_add)[6]) final;
        virtual uint8_t* GetMacAddr( ) final;

} __attribute__((aligned(8)));

/* ************ */
class LoopbackInterface : public Interface {

    public:
        LoopbackInterface(std::string ifname);
        virtual ~LoopbackInterface();

} __attribute__((aligned(8)));
