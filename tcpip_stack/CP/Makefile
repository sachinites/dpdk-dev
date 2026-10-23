LIB     := libcp.a
SRC     := cp_ctx.cpp \
           LinuxInterface.cpp \
		   Interface/Interface.cpp \
		   Interface/InterfaceApi.cpp \
		   Interface/InterfaceCli.cpp \
		   nwcli.cpp 

OBJ     := $(SRC:.cpp=.o)

CXX     := g++
PKG_CONFIG ?= pkg-config
DPDK_CFLAGS := $(shell $(PKG_CONFIG) --cflags libdpdk)
CXXFLAGS := -O2 -Wall -Wextra $(DPDK_CFLAGS)

AR      := ar
ARFLAGS := rcs

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
