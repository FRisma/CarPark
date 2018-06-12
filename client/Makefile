CFLAGS := -Wall -Wextra -Wpedantic -Wchkp -Wuninitialized -Winit-self -g
TARGETS := client
CC := gcc $(CFLAGS)
SFLAGS := -lm

all: $(TARGETS)

client: main.c configServer.c establishConnection.c createTicket.c deleteTicket.c statusTicket.c\
	dispatch.c serialize.c constants.c\
	utils/optionsHandler.c utils/parse.c\
	socket/ipv4.c socket/ipv6.c socket/unspec.c socket/protocol_handler.c
	$(CC) -o $@ $^ -lm

clean:  
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./client

strace: all
	strace -f ./client -c
