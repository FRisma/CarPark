CFLAGS := -Wall -Wextra -g -L/home/ubuntu/compu2/CarPark/jsonParser/lib
TARGETS := client
CC := gcc $(CFLAGS)

all: $(TARGETS)

client: main.c configServer.c establishConnection.c createTicket.c deleteTicket.c statusTicket.c constants.c\
	utils/optionsHandler.c utils/parse.c\
	socket/ipv4.c socket/ipv6.c socket/unspec.c socket/protocol_handler.c
	$(CC) -o $@ $^

clean:  
	rm -rfv *.o *~ $(TARGETS) *.dSYM

test: all
	./client

strace: all
	strace -f ./client -c
