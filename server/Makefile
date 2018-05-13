CFLAGS := -Wall -Wextra -g -pthread
TARGETS := server
CC := gcc $(CFLAGS)

all: $(TARGETS)

server: main.c parkingManager.c configServer.c runServer.c threadWork.c\
	utils/optionsHandler.c utils/parse.c\
	socket/ipv4.c socket/ipv6.c socket/unspec.c socket/protocol_handler.c\
	-lrt
	$(CC) -o $@ $^

clean:  
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./server

strace: all
	strace -f ./server
