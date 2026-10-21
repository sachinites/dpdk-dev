APP     := tcpstack.exe
SRC     := main.cpp
OBJ     := main.o

CP_DIR  := CP
DP_DIR  := DP
LIBCP   := $(CP_DIR)/libcp.a
LIBDP   := $(DP_DIR)/libdp.a

CXX     := g++
PKG_CONFIG ?= pkg-config
DPDK_CFLAGS := $(shell $(PKG_CONFIG) --cflags libdpdk)
DPDK_LIBS   := $(shell $(PKG_CONFIG) --libs libdpdk)
CXXFLAGS := -O2 -Wall -Wextra -I$(CP_DIR) -I$(DP_DIR) $(DPDK_CFLAGS)

.PHONY: all clean $(LIBCP) $(LIBDP)

all: $(APP)

$(APP): $(OBJ) $(LIBCP) $(LIBDP)
	$(CXX) $(OBJ) $(LIBCP) $(LIBDP) -o $@ $(DPDK_LIBS)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBCP):
	$(MAKE) -C $(CP_DIR)

$(LIBDP):
	$(MAKE) -C $(DP_DIR)

clean:
	rm -f $(OBJ) $(APP)
	$(MAKE) -C $(CP_DIR) clean
	$(MAKE) -C $(DP_DIR) clean
