LIB     := libdp.a
SRC     := dp_ctx.cpp \
           dp-program/dp_prog_api.cpp \
           ../dpal/cp2dp.cpp
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
