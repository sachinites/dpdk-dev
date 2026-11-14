#ifndef __INTERFACE_API_H__
#define __INTERFACE_API_H__

#include "InterfaceFwd.h"

int create_loopback_interface(const char *if_name);
int delete_loopback_interface(const char *if_name);

#endif
