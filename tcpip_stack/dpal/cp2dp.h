#ifndef __CP2DP__
#define __CP2DP__

typedef struct dp_msg_ dp_msg_t;
class Interface;


void cp2dp_submit(dp_msg_t *dp_msg);

void
cp2dp_interface_create(Interface& phy_intf);

void
cp2dp_interface_delete(Interface& phy_intf);

#endif 
