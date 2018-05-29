#include "app.h"
#include "constants.h"

#include <stdio.h>
#include <unistd.h>
#include "../jsonParser/include/jansson.h"

#define debug 0

void createTicket(int socketDescriptor) {

	char buff[512];

	printf("La constante es: %s ----\n",HTTP_GET);

	char *request = (char *)malloc(512);
	char *body = (char *)malloc(256);

	snprintf(request,512,HTTP_POST,"/slots",CT_JSON,"256","{'prueba' : 'json'}");
	json_t *root;
	json_error_t error;

	//Hace la llamada
	//
	//Una vez que tengo la respuesta llamo a root=json_loads(response) para decodificar la respuesta
	//free(response)
	//if (!root) {
	// perror(json_loads); //Imprimir error
	// return -1;
	// }

	write(socketDescriptor,request,strlen(request));
	while( read(socketDescriptor,buff,512) ) {
		printf("S: %s\n",buff);
	}
	//HAcer un post con un hash para identificar al nuevo usuario
	//deberia tener un id de app cliente, unico para cada instancia, de esa manera
	//en el server, guardamos tambien este id de puerta de ingreso, y nos permite
	//tener estadisticas de cual es la puerta mas usada.
}
