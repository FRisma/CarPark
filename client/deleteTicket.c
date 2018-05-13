#include "app.h"

#include <stdio.h>
#include <unistd.h>

#define debug 0

void deleteTicket(int socketDescriptor, char *slotId) {

	// Deberia hacer un delete de la posicion, basado en el slotId.
	char buff[512];
	write(socketDescriptor,"C: Hola, delete\n",17);
	while( read(socketDescriptor,buff,512) ) {
		printf("S: %s\n",buff);
	}
}
