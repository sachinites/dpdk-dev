LIB     := libstd.a

CC      := g++
CFLAGS  := -O2 -Wall -Wextra -I. -Igluethread -Istack -IEventDispatcher

SRCS    := std_lib_init.cpp \
           gluethread/glthread.cpp \
           stack/stack.cpp \
	   EventDispatcher/LockLessEvDis.cpp

OBJS    := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
