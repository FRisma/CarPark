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
	//slot slotArray[100];

	// Crear la estructura para los argumentos del hilo
	threadData tdata = {0};
	tdata.mqd = conf->mqd;
	// Create a mutex for launching concurrent threads (by default its created with value 1)
	pthread_mutex_init(&tdata.sincro, NULL);
	
	printf("Starting server...\n\tListening on socket:%d\tServer port: %s\n", socket, conf->port);
	free(conf);
	if ( 0 > listen(socket,5) ) {
		perror("listen");
		return -1;
	}
	
	int i = 1;
	int nsd;
	while ( (nsd = accept(socket, (struct sockaddr *)&client, &cli_size)) != -1 ) {
		if (nsd == -1) {
			perror("accept");
			// it would be much better to have a handle_error to check the root cause of the error (see bind(2))
			return -1;
		} else {
			printf("Client IP: %s Socket: %d Count: %d\n", inet_ntoa(client.sin_addr), nsd, i);
			tdata.csd = nsd;

			if (pthread_create(&tid1, NULL, threadWork,(void *)&tdata) != 0) {
				perror("pthread_create");
			}
		}
		i++;
	}

	close(socket);
	close(tdata.mqd);
	return 0;
}
