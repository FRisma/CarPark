CFLAGS := -Wall -Wextra -Wpedantic -Wchkp -Wuninitialized -Winit-self -g -pthread
TARGETS := server
CC := gcc $(CFLAGS)

all: $(TARGETS)

server: main.c parkingManager.c configServer.c\
	createLocations.c freeLocations.c runServer.c threadWork.c requestHeader.c\
	checkin.c checkout.c status.c slotIdFromURI.c deserialize.c serialize.c billing.c\
	response.c dispatchResponse.c logActivity.c listLocations.c printLocation.c\
	utils/optionsHandler.c utils/parse.c\
	socket/ipv4.c socket/ipv6.c socket/unspec.c socket/protocol_handler.c
	$(CC) -o $@ $^ -lrt -lm

clean:  
	rm -rfv *.o *~ $(TARGETS) *.dSYM core

test: all
	./server

strace: all
	strace -f ./server
