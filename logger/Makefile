CFLAGS := -Wall -Wextra -g
TARGETS := logger
CC := gcc $(CFLAGS)

all: $(TARGETS)

logger: main.c -lrt
	$(CC) -o $@ $^

clean:
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./logger

strace: all
	strace -f ./logger
