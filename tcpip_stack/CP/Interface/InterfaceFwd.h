#ifndef __INTERFACE_FWD__
#define __INTERFACE_FWD__

#include <memory>

class PhysicalInterface;
class VlanInterface;
class LoopbackInterface;
class RmacInterface;
class VlanFloodInterface;
class Interface ;

using InterfaceP = std::shared_ptr<Interface>;
using VlanInterfaceP = std::shared_ptr<VlanInterface>;
using PhysicalInterfaceP = std::shared_ptr<PhysicalInterface>;
using RmacInterfaceP = std::shared_ptr<RmacInterface>;
using VlanFloodInterfaceP = std::shared_ptr<VlanFloodInterface>;
using InterfaceLoP = std::shared_ptr<LoopbackInterface>;


#endif 