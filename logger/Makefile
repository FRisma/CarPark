CFLAGS := -Wall -Wextra -Wpedantic -Wchkp -Wuninitialized -Winit-self -g
LDLIBS=-lrt
TARGETS := logger
CC := gcc $(CFLAGS)

all: $(TARGETS)

logger: main.c 
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./logger

strace: all
	strace -f ./logger
