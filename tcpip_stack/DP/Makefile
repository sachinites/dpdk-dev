LIB     := libdp.a
SRC     := dp_ctx.cpp \
           dp-program/dp_prog_api.cpp \
           Interface/dp_intf_store.cpp
OBJ     := $(SRC:.cpp=.o)

CXX     := g++
CXXFLAGS := -O2 -Wall -Wextra -I. -I../libs

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
