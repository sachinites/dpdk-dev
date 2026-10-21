LIB     := libcp.a
SRC     := cp_ctx.cpp
OBJ     := $(SRC:.cpp=.o)

CXX     := g++
CXXFLAGS := -O2 -Wall -Wextra

AR      := ar
ARFLAGS := rcs

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
