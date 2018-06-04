#include "app.h"
#include "socket/mySocket.h"

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 1

int parkingManager(char *configFile) {

	// Hacer todas las configuraciones iniciales para el parking server
	if (debug) printf("Archivo de configuracion %s\n", configFile);
	serverConf *srvConf = (serverConf *)malloc(sizeof(serverConf));

	if ( 0 > configServer(configFile, srvConf->port,srvConf->mqName) ) {
		free(srvConf);
		perror("No se pudo configurar el servidor");
		return -1;
	}
	srvConf->protocol = 4; // Esto deberia ser parametrizable

	// Arranco el proceso de logging indicandole el nombre de la cola como argumento
	//startLoggingProcess(srvConf->mqName);

	// Creo el socket
	if ( 0 > protocol_handler(srvConf->protocol, &srvConf->socketDescriptor, srvConf->port) ) {
		perror("No se pudo configurar el socket");
		close(srvConf->socketDescriptor);
		free(srvConf);
		return -1;
	}

	// Creo la cola de mensajes
	srvConf->mqd = -1;
	printf("Abro la cola %s\n",srvConf->mqName);
	if ( -1 == (srvConf->mqd = mq_open(srvConf->mqName, O_WRONLY)) ) {
		perror("mq_open");
		printf("The message queue %s was not found, so no activity logging will be done",srvConf->mqName);
	}

	//Iniciar el parking server
	if (debug) printf("Iniciando parking server - Protocol: ipv%d, Socket: %d, Port: %s\n", srvConf->protocol, srvConf->socketDescriptor, srvConf->port);
	if ( 0 > runServer(srvConf) ) {
		perror("Algo salio mal en runServer");
		mq_close(srvConf->mqd);
		close(srvConf->socketDescriptor);
		free(srvConf);
		return -1;
	}
	
	puts("Saliendo");
	return 0;
}
