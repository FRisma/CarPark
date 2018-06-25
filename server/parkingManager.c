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

	// Creo el socket
	if ( 0 > protocol_handler(srvConf->protocol, &srvConf->socketDescriptor, srvConf->port) ) {
		perror("No se pudo configurar el socket");
		close(srvConf->socketDescriptor);
		free(srvConf);
		return -1;
	}

	// Creo la cola de mensajes
	srvConf->mqd = -1;
	if (debug) printf("Abro la cola %s\n",srvConf->mqName);
	if ( -1 == (srvConf->mqd = mq_open(srvConf->mqName, O_WRONLY)) ) {
		perror("mq_open");
		printf("The message queue %s was not found, so no activity logging will be done\n",srvConf->mqName);
	}

	//Iniciar el parking server
	if (debug) printf("Starting parking server - Protocol: ipv%d, Socket:%d, Port:%s MQD:%d\n", srvConf->protocol, srvConf->socketDescriptor, srvConf->port, srvConf->mqd);
	if ( 0 > runServer(srvConf) ) {
		perror("Algo salio mal en runServer");
		mq_close(srvConf->mqd);
		close(srvConf->socketDescriptor);
		free(srvConf);
		return -1;
	}

	puts("Saliendo");
	mq_close(srvConf->mqd);
	close(srvConf->socketDescriptor);
	free(srvConf);
	return 0;
}
