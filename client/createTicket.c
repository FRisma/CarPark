#include "app.h"
#include "constants.h"
#include "../jsonParser/include/jansson.h"

#include <stdio.h>
#include <unistd.h>

#define debug 0

int createTicket(int socketDescriptor) {

	char buff[512];

	char *httpRequest = (char *)malloc(512);
	char *httpResponse= (char *)malloc(512);
	char *body = (char *)malloc(256);

	//slot s1 = (slot *)malloc(sizeof(slot));
	slot s1;
	s1.idCli = 1;
	/* This should be returned by the server, now it´s for testing purposes only */	
	s1.floor = 2;
	s1.offset = 4;
	/* end of testing data */
	if ( -1 == serialize(s1,body) ) {
		free(httpRequest);
		free(body);
		perror("Something went wrong serializing");
		return -1;
	}

	//json_t *body = json_pack("{sisi}", "foo", 42, "bar", 7);
	//json_t *body = json_pack("i", 7);

	if (debug) printf("El body serializado es %s\n", body);

	snprintf(httpRequest,512,HTTP_POST,"/slots",CT_JSON,"256",body);
	//json_t *root;
	//json_error_t error;

	//Hace la llamada
	if (-1 == dispatch(socketDescriptor,httpRequest,httpResponse) ) {
		free(httpRequest);
		free(body);
		return -1;
	}
	free(httpRequest);
	free(body);
	
	//root=json_loads(response, 0, &error); //decodificar la respuesta
	//free(response)
	/*if (!root) {
		printf("Parse error on line: %d: %s\n", error.line, error.text);
		//perror(json_loads); //Imprimir error
		return -1;
	}*/

	//HAcer un post con un hash para identificar al nuevo usuario
	//deberia tener un id de app cliente, unico para cada instancia, de esa manera
	//en el server, guardamos tambien este id de puerta de ingreso, y nos permite
	//tener estadisticas de cual es la puerta mas usada.
	
	return 0;
}
