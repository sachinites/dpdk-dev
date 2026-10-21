APP     := tcpstack.exe
SRC     := main.cpp
OBJ     := main.o

CXX     := g++
PKG_CONFIG ?= pkg-config
DPDK_CFLAGS := $(shell $(PKG_CONFIG) --cflags libdpdk)
DPDK_LIBS   := $(shell $(PKG_CONFIG) --libs libdpdk)
CXXFLAGS := -O2 -Wall -Wextra $(DPDK_CFLAGS)

.PHONY: all clean

all: clean $(APP)

$(APP): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(DPDK_LIBS)
	rm -f $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(APP)
