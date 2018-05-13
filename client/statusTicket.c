#include "app.h"

#include <stdio.h>
#include <unistd.h>

#define debug 0

void statusTicket(int socketDescriptor, char *slotId) {

	char buff[512];

	//Deberia hacer un GETchar buff[512];
	write(socketDescriptor,"C: Hola, status\n",17);
	while( read(socketDescriptor,buff,512) ) {
		printf("S: %s\n",buff);
	}

}
