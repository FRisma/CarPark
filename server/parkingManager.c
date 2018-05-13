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
		free(srvConf);
		close(srvConf->socketDescriptor);
		return -1;
	}

	// Creo la cola de mensajes
	mqd_t mq;
	if ( -1 == (mq = mq_open("/carpark_log_queue", O_WRONLY)) ) {
		perror("mq_open");
		printf("The message queue %s was not found, so no activity logging will be done","/carpark_log_queue");
	}

	if (mq > 0) {
		mq_send(mq, "Hola desde el server", 21, 0);
	}


	//Iniciar el parking server
	if (debug) printf("Iniciando parking server - Protocol: ipv%d, Socket: %d, Port: %s\n", srvConf->protocol, srvConf->socketDescriptor, srvConf->port);
	if ( 0 > runServer(srvConf) ) {
		perror("Algo salio mal en runServer");
		free(srvConf);
		mq_close(mq);
		close(srvConf->socketDescriptor);
		return -1;
	}
	

	puts("Saliendo");
	free(srvConf);
	mq_close(mq);
	close(srvConf->socketDescriptor);

	return 0;
}
