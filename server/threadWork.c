#include "app.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//static char * const e403_code="HTTP/1.0 403 FORBIDDEN\r\nContent-Type: text/html\n\n<html>\n<body>\n403 Access Denied\n</body>\n</html>";
static char * const e403_message="<html>\n<body>\n403 You don't have permission to view this file\n</body>\n</html>";
static char * const e404_message="<html>\n<body>\n404 File not found\n</body>\n</html>";
static char * const e500_message="<html>\n<body>\n500 Something went terribly wrong\n</body>\n</html>";

static char * const errorTemplate="HTTP/1.0 %s\r\nContent-Type: text/html\n\n%s";

#define debug 0

void* threadWork(void *data) {
	
	// Set this thread as detached
	pthread_detach(pthread_self());
	
	/* Map structure values into local variables */
	threadData *arg = (threadData *)data;
	int sd = arg->csd;
	mqd_t mq = arg->mqd;
	pthread_mutex_t *mutex = arg->sincro;
	
	struct slot *start = arg->start; // HEAD
	//free(data);	
	
	if (debug) {
		printf("ThreadArgs->connectionSD:%d threadArgs->mqd:%d\n",sd, mq);
		struct slot *tmp = start;
		do {
			printf("T Nodo: %li\n", tmp->id);
			tmp->id = 3;
			tmp = tmp->next;
		}while(tmp->next != NULL);
	}
	
	http_request *req = (http_request *)malloc(sizeof(http_request));
	if ( 0 > requestHeader(sd,req)) {
		close(sd);
		free(req);
		pthread_exit(NULL);
	}

	if (debug) {
		printf("Request\n\tMT: %d\n\tRS: %s\n\tCT: %s\n\tCL: %li",req->method, req->resource, req->content_type, req->content_length);
		if (req->body) printf("Request - BO: %s\n",req->body);
	}
	//free(req->body);

	struct slot *result = NULL; // Pointer to the result
	// Hacer el trabajo
	switch (req->method) {
		case POST: //buscar un lugar libre
			checkin(start,result,mutex);
			if (NULL == result) {
				puts("No more places");
			} else {
				printf("Asignado Nodo: %li disp:%d hora:%s\n hilo:%li\n", result->id, result->available, asctime(result->checkInTime), pthread_self());
			}
			break;
		case GET: // Si viene un id en resource es porque esta consultando el estado
			puts("Status de una reserva");
			//status();
			break;
		case DELETE: // Si o si el delete tiene que venir acompaÃado de un req->resource con un id
			puts("Delete una reserva");
			//checkout(node, mutex);
			break;
		default:
			puts("Not implemented yet");
			//Todavia no se implementa
	}
	
	//createResponse();

	if ( 0 > write(sd,"HTTP 200 OK\r\n\r\n",15) ) {
			perror("Thread - write");
			free(req);
			close(sd);
			pthread_exit(NULL);
	}


	if ( -1 == close(sd) ) { perror("close"); }

	/* using mq_timesend because if the queue is full we don't want to block the thread for a long period of time */
	if (0 < mq) {
		struct timespec waitTime = {0};
		waitTime.tv_sec = 1; 	/* seconds */
		waitTime.tv_nsec = 0; 	/* nano seconds */
		if ( -1 == mq_timedsend(mq, "mensaje desde el hilo", 21, 0, &waitTime) ) {
			perror("mq_timesend");
		}
	}

	free(req);
	pthread_exit(NULL);
}
	
	/*							
		switch ( request(buffer,arg->droot,newSocketDescriptor) ){
			case 200: // Termina exitosamente
				if (debug) puts("200OK File served");
				break;
			case 404: // Archivo no encontrado
				if (debug) puts("File not found");
				if ( snprintf(responseHeader,strlen(e404_code)+strlen(e404_message)+37,errorTemplate,e404_code,e404_message) < 0 ) {
					perror("snprintf error404");
				}
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 404");
				}
				break;
			case 403: // Forbidden
				if (debug) puts("File permissions issue");
				if ( snprintf(responseHeader,strlen(e403_code)+strlen(e403_message)+37,errorTemplate,e403_code,e403_message) < 0 ) {
					perror("snprintf erro403");
				}		
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 403");
				}
				break;
			case 500: // Problemas del server
			default:
				if (debug) puts("Something went wrong");
				if ( snprintf(responseHeader,strlen(e500_code)+strlen(e500_message)+37,errorTemplate,e500_code,e500_message) < 0 ) {
					perror("snprintf error500");
				}
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 500");
				}
				break;
		}

		*/
