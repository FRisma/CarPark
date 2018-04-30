CFLAGS := -Wall -Wextra -g
TARGETS := client
CC := gcc $(CFLAGS)

all: $(TARGETS)

client: main.c configServer.c establishConnection.c\
	utils/optionsHandler.c utils/parse.c\
	socket/ipv4.c socket/ipv6.c socket/unspec.c socket/protocol_handler.c
	$(CC) -o $@ $^

clean:  
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./client

strace: all
	strace -f ./client -c
