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

	pthread_t tid1;
	int socket = conf->socketDescriptor;	

	struct sockaddr_in client;
	socklen_t cli_size = sizeof(struct sockaddr_in);	
	
	// Crear el array de estructuras
	struct slot *locStart = NULL;
	if (-1 == createLocations(&locStart)) {
		write(STDERR_FILENO,"Error: createLocations\n",23);
		return -1;
	}

	// Create a mutex for launching concurrent threads (by default its created with value 1)
	// this should be in the heap not in the stack, so every thread has access to it and share the value
	pthread_mutex_t *mutex = NULL;
	if ( NULL ==  (mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t))) ) {
		perror("malloc");
		freeLocations(locStart);
		return -1;
	}
	pthread_mutex_init(mutex, NULL);

	printf("Starting server...\n\tListening on socket:%d\tServer port: %s\n", socket, conf->port);
	free(conf);
	if ( 0 > listen(socket,5) ) {
		perror("listen");
		freeLocations(locStart);
		free(mutex);
		return -1;
	}

	int nsd = -1;
	while ( (nsd = accept(socket, (struct sockaddr *)&client, &cli_size)) != -1 ) {
		if (-1 == nsd) {
			perror("accept"); // it would be much better to have a handle_error to check the root cause of the error (see bind(2))
			freeLocations(locStart);
			free(mutex);
			return -1;
		} else {
			printf("Client IP: %s Socket: %d\n", inet_ntoa(client.sin_addr), nsd);
			
			// Crear la estructura para los argumentos del hilo
			threadData *tdata = NULL;
			if ( NULL != (tdata = (threadData *)malloc(sizeof(threadData))) ) {
				memset(tdata,'\0',sizeof(threadData));
				tdata->mqd = conf->mqd;
				tdata->csd = nsd;
				tdata->sincro = mutex;
				tdata->start = locStart;

				// Start thread
				if (pthread_create(&tid1, NULL, threadWork, (void *)tdata) != 0) {
					perror("pthread_create");
					free(tdata);
				}
			} else {
				perror("malloc"); //No libero ni salgo ya que el probelma de no poder
				//asignar memoria podria ser temporal, asi es que simplemente dejamos
				//a este cliente sin atender
			}
		}
	}

	close(socket);
	free(mutex);
	freeLocations(locStart);
	return 0;
}
