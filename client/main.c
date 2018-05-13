#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

int main (int argc, char *const argv[]) {

	puts("\t\t ..:: Trabajo Final - Computacion II - CarPark Client ::..");

	int socketDescriptor = 0;

	// Manejo de opciones
	configuration *conf = (configuration *)malloc(sizeof(configuration));
	optionsHandler(argc, argv, conf);

	// Configuro el cliente desde archivo
	clientConf *servParams = (clientConf *)malloc(sizeof(configuration));
	if ( 0 > configServer("servidor.cfg",servParams->ip,servParams->port) ) {
		free(conf);
		free(servParams);
		return -1;
	}

	// Abro conexion con el socket
	establishConnection(&socketDescriptor,servParams);

	// Hago el trabajo
	switch (conf->opt) {
		case 'c':
			if (debug) puts("Crear");
			createTicket(socketDescriptor);
			break;
		case 'd':
			if (debug) puts("Eliminar");
			deleteTicket(socketDescriptor,conf->slotId);
			break;
		case 's':
			if (debug) puts("Status");
			statusTicket(socketDescriptor,conf->slotId);
			break;
	}

	// Muero
	puts("Adios");
	free(conf);
	free(servParams);

	return 0;
}
