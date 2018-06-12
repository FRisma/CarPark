#include "app.h"
#include "constants.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int createTicket(int socketDescriptor) {

	char httpRequest[512];
	char httpResponse[512];
	char body[256];

	slot s1;
	s1.idCli = 1;
	
	if ( -1 == serialize(s1,body) ) {
		perror("Something went wrong serializing");
		return -1;
	}
	if (debug) printf("Serialized body %s\n", body);

	char resource[] = "/slots";
	size_t body_len = strnlen(body,sizeof(body)) + 1;
	int blDigits = floor(log10(abs(body_len))) + 1;
	snprintf(httpRequest, strlen(HTTP_POST) + strlen(resource) + strlen(CT_JSON) + blDigits + body_len + 1, HTTP_POST, resource, CT_JSON, body_len, body);

	//Hace la llamada
	if (-1 == dispatch(socketDescriptor,httpRequest,httpResponse) ) {
		return -1;
	}
	
	//HAcer un post con un hash para identificar al nuevo usuario
	//deberia tener un id de app cliente, unico para cada instancia, de esa manera
	//en el server, guardamos tambien este id de puerta de ingreso, y nos permite
	//tener estadisticas de cual es la puerta mas usada.
	
	return 0;
}
