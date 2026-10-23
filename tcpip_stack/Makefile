APP     := tcpstack.exe
SRC     := main.cpp
OBJ     := main.o

CP_DIR  := CP
DP_DIR  := DP
CLI_DIR := MgmtPlane/CLIBuilder
LIBS_DIR := libs

LIBCP   := $(CP_DIR)/libcp.a
LIBDP   := $(DP_DIR)/libdp.a
LIBCLI  := $(CLI_DIR)/libclibuilder.a
LIBSTD  := $(LIBS_DIR)/libstd.a

CXX     := g++
PKG_CONFIG ?= pkg-config
DPDK_CFLAGS := $(shell $(PKG_CONFIG) --cflags libdpdk)
DPDK_LIBS   := $(shell $(PKG_CONFIG) --libs libdpdk)
CXXFLAGS := -O2 -Wall -Wextra -I$(CP_DIR) -I$(DP_DIR) -I$(CLI_DIR) -I$(LIBS_DIR) $(DPDK_CFLAGS)

.PHONY: all clean $(LIBCP) $(LIBDP) $(LIBCLI) $(LIBSTD)

all: $(APP)

$(APP): $(OBJ) $(LIBCP) $(LIBDP) $(LIBCLI) $(LIBSTD)
	$(CXX) $(OBJ) $(LIBCP) $(LIBDP) $(LIBCLI) $(LIBSTD) -o $@ $(DPDK_LIBS) -lncurses

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBCP):
	$(MAKE) -C $(CP_DIR)

$(LIBDP):
	$(MAKE) -C $(DP_DIR)

$(LIBCLI):
	$(MAKE) -C $(CLI_DIR)

$(LIBSTD):
	$(MAKE) -C $(LIBS_DIR)

clean:
	rm -f $(OBJ) $(APP)
	$(MAKE) -C $(CP_DIR) clean
	$(MAKE) -C $(DP_DIR) clean
	$(MAKE) -C $(CLI_DIR) clean
	$(MAKE) -C $(LIBS_DIR) clean
