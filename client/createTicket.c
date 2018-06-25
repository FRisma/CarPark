#include "app.h"
#include "constants.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define debug 0

const char *kClientName = "Puerta1";

int createTicket(int socketDescriptor) {

	char httpRequest[512];
	char httpResponse[512]; //Esto tiene que ser un puntero para poder escribirlo en la funcion dispach
	char body[256];

	slot s1 = {'\0'};
	strncpy(s1.idCli,kClientName,strnlen(kClientName,10));
	
	if ( -1 == serialize(s1,body) ) {
		perror("Something went wrong serializing");
		return -1;
	}
	if (debug) printf("Serialized body %s\n", body);

	char resource[] = "/slots";
	size_t body_len = strnlen(body,sizeof(body)) + 1;
	int blDigits = floor(log10(abs(body_len))) + 1;
	snprintf(httpRequest, strlen(HTTP_POST) + strlen(resource) + strlen(CT_TEXT) + blDigits + body_len + 1, HTTP_POST, resource, CT_TEXT, body_len, body);

	//Hace la llamada
	if (-1 == dispatch(socketDescriptor,httpRequest,httpResponse) ) {
		return -1;
	}

	// Convertir el http response a una estructura slot - En este punto ya no es necesario debido a que mostramos por
	// pantalla la respuesta y termina. 
	printf("Nueva posicion asignada\n%s\n",httpResponse);
	
	return 0;
}
