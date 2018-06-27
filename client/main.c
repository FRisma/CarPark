#include "app.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

int main (int argc, char *const argv[]) {

	puts("\t\t ..:: Trabajo Final - Computacion II - CarPark Client ::..");

	int socketDescriptor = 0;

	// Manejo de opciones
	configuration *conf = NULL;
	if ( NULL == (conf=(configuration *)malloc(sizeof(configuration))) ) {
		perror("malloc");
		return -1;
	}
	memset(conf,'\0',sizeof(configuration));
	optionsHandler(argc, argv, conf);

	// Configuro el cliente desde archivo
	clientConf *servParams = NULL;
	if ( NULL == (servParams =(clientConf*)malloc(sizeof(clientConf))) ) {
		perror("malloc");
		free(conf);
		return -1;
	}
	memset(servParams,'\0',sizeof(clientConf));
	if ( 0 > configServer("servidor.cfg",servParams->ip,servParams->port) ) {
		free(conf);
		free(servParams);
		return -1;
	}

	// Abro conexion con el socket
	if ( -1 == establishConnection(&socketDescriptor,servParams) ) {
		perror("establishConnection");
		free(servParams);
		return -1;
	}
	free(servParams);

	// Hago el trabajo
	switch (conf->opt) {
		case 'c':
			if (debug) puts("Crear");
			if ( -1 == createTicket(socketDescriptor) ) {
				write(STDERR_FILENO,"Error: createTicket\n",20);
				close(socketDescriptor);
				free(conf);
				return -1;
			}
			break;
		case 'd':
			if (debug) puts("Eliminar");
			if ( -1 == deleteTicket(socketDescriptor,conf->slotId) ) {
				write(STDERR_FILENO,"Error: createTicket\n",20);
				close(socketDescriptor);
				free(conf);
				return -1;
			}
			break;
		case 's':
			if (debug) puts("Status");
			if ( -1 == statusTicket(socketDescriptor,conf->slotId) ) {
				write(STDERR_FILENO,"Error: createTicket\n",20);
				close(socketDescriptor);
				free(conf);
				return -1;
			}
			break;
		default:
			write(STDERR_FILENO,"Metodo de trabajo invalido\n",27);
			close(socketDescriptor);
			free(conf);
			return -1;
	}

	// Muero
	close(socketDescriptor);
	free(conf);

	return 0;
}
