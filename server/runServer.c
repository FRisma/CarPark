#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>

#include <pthread.h>
#include <netdb.h>

#define debug 0

int runServer(serverConf *conf) {

	if (debug) puts("runServer.c");

	pthread_t tid1;

	int socket = conf->socketDescriptor;	

	struct sockaddr_in client;
	socklen_t cli_size = sizeof(struct sockaddr_in);	
	
	// Crear el array de estructuras
	slot slotArray[100];

	// Create a mutex
	
	// Crear la estructura para los argumentos del hilo
	threadData tdata;

	// Create a mutex for launching concurrent threads (by default its created with value 1)
	pthread_mutex_init(&tdata.sincro, NULL);
	
	printf("Starting server...\n\tListening on socket:%d\tServer port: %s\n", socket, conf->port);
	if ( 0 > listen(socket,5) ) {
		perror("listen");
		return -1;
	}

	for (;;) {
		if ( (tdata.csd = accept(socket,(struct sockaddr *)&client,&cli_size)) == -1 ) {
			puts("something went wrong while serving the client");
			perror("accept");
			return -1;
		} else {
			puts("new client");
			printf("Client IP: %s Socket: %d\n", inet_ntoa(client.sin_addr), tdata.csd);

			if (pthread_create(&tid1, NULL, threadWork,(void *)&tdata) != 0) {
				perror("pthread_create");
				//return -1;
			}
			close(tdata.csd);
		}
	}

	return 0;
}
