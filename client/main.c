#include "app.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 1

int main (int argc, char *const argv[]) {

	puts("Trabajo Final - Computacion II - CarPark Client");

	int socketDescriptor = 0;

	// Manejo de opciones
	configuration *conf = (configuration *)malloc(sizeof(configuration));
	optionsHandler(argc, argv, conf);

	// Chequear el correcto estado de las opciones
	switch (conf->opt) {
		case 'c':
			puts("Creando un ticket");
			break;
		case 'd':
			puts("Eliminando un ticket");
			break;
		case 's':
			puts("Estado de un ticket");
			break;
	}

	// Configuro el cliente desde archivo
	clientConf *servParams = (clientConf *)malloc(sizeof(configuration));
	configServer("servidor.cfg",servParams->serverIp,servParams->port);

	// Abro conexion con el socket
//	establishConnection(&socketDescriptor,servParams);
	
	// Muero
	free(conf);
	free(servParams);
	return 0;
}
