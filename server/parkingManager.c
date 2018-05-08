#include "app.h"
#include "socket/mySocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

int parkingManager(char *configFile) {

	// Hacer todas las configuraciones iniciales para el parking server
	printf("Archivo de configuracion %s", configFile);
	serverConf *srvConf = (serverConf *)malloc(sizeof(serverConf));

	if ( 0 > configServer(configFile, srvConf->port) ) {
		free(srvConf);
		perror("No se pudo configurar el servidor");
		return -1;
	}
	srvConf->protocol = 4; // Esto deberia ser parametrizable

	// Crear el array de estructuras
	slot slotArray[100];

	// Creo la cola de mensajes

	// Creo el socket
	if ( 0 > protocol_handler(srvConf->protocol, &srvConf->socketDescriptor, srvConf->port) ) {
		perror("No se pudo configurar el socket");
		free(srvConf);
		close(srvConf->socketDescriptor);
		return -1;
	}
	printf("El socket fd es: %d", srvConf->socketDescriptor);

	puts("A punto de arrancar");
	//Iniciar el parking server
	if (debug) printf("Iniciando parking server - Protocol: ipv%d, Socket: %d, Port: %s", srvConf->protocol, srvConf->socketDescriptor, srvConf->port);
	puts("2 A punto de arrancar");
	if ( 0 > runServer(srvConf) ) {
		free(srvConf);
		close(srvConf->socketDescriptor);
		//close(messageQueue);
		perror("Algo salio mal en runServer");
		return -1;
	}
	

	// Cierro la cola de mensajes
	// Close socket descriptor
	close(srvConf->socketDescriptor);

	puts("Saliendo");
	return 0;
}
