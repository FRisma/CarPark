#include "app.h"
#include "constants.h"

#include <stdio.h>
#include <unistd.h>

#define debug 0

void createTicket(int socketDescriptor) {

	char buff[512];

	printf("La constante es: %s ----\n",HTTP_GET);

	write(socketDescriptor,"C: Hola, create\n",17);
	while( read(socketDescriptor,buff,512) ) {
		printf("S: %s\n",buff);
	}
	//HAcer un post con un hash para identificar al nuevo usuario
	//deberia tener un id de app cliente, unico para cada instancia, de esa manera
	//en el server, guardamos tambien este id de puerta de ingreso, y nos permite
	//tener estadisticas de cual es la puerta mas usada.
}
