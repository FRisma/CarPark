#include "app.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define debug 1

void* threadWork(void *data) {
	
	// Set this thread as detached
	pthread_detach(pthread_self());
	
	/* Map structure values into local variables */
	threadData *arg = (threadData *)data;
	int sd = arg->csd;
	mqd_t mq = arg->mqd;
	pthread_mutex_t *mutex = arg->sincro;
	
	struct slot *start = arg->start; // HEAD
	free(data); //No debo liberar data en este punto porque tengo otros punteros que apuntan a su contenido
	
	if (debug) {
		printf("ThreadArgs->connectionSD:%d threadArgs->mqd:%d\n",sd, mq);
		struct slot *tmp = start;
		do {
			printf("T Nodo: %li\n", tmp->id);
			tmp = tmp->next;
		}while(tmp->next != NULL);
	}

	/* ------------------- Request Handing ------------------------- */
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

	/* ------------------- Response Handling ------------------------- */
	int http_resp_code = 0;
	struct slot *result = (slot*)malloc(sizeof(slot)); // Pointer to the result
	memset(result,'\0',sizeof(result));
	
	switch (req->method) {
		case POST:
			deserialize(req->body, &result); // Map body to slot
			checkin(start,&result,mutex);
			if (NULL == result) { //TODO this if is not working
				puts("No more places");
				http_resp_code = kHttp204;
			} else {
				printf("Asignado Nodo:%li disp:%d hora:%s hilo:%li\n", result->id, result->available, asctime(result->checkInTime), pthread_self());
				http_resp_code = kHttp201;
			}
			break;
		case GET:
			if (debug) printf("Status reserva: %s\n",req->resource);
			long int idGet = slotIdFromURI(req->resource);
			if (-1 != idGet) {
				status(start,&result,idGet);
				if (0 < result->id) {
					billing(result);
					http_resp_code = kHttp200;
					if (debug) printf("GET Result Nodo:%li idCli:%s Disp:%d\n", result->id, result->idCli, result->available);
				} else { // ID not found
					write(STDERR_FILENO,"ID not found\n",13);
					http_resp_code = kHttp404;
				}
			} else { // ID invalid
				http_resp_code = kHttp400;
				write(STDERR_FILENO,"No valid slotId\n",16);
			}
			break;
		case DELETE:
			if (debug) printf("Delete reserva: %s\n",req->resource);
			long int idDelete = slotIdFromURI(req->resource);
			if (-1 != idDelete) {
				slot pos = {'\0'};
				pos.id = idDelete;
				checkout(start, &pos, mutex); //TODO this is failing
				if (0 < result->id) {
					billing(result);
					http_resp_code = kHttp200;
					if (debug) printf("serialize Nodo:%li idCli:%s Disp:%d\n", result->id, result->idCli, result->available);
				} else { // ID not found
					write(STDERR_FILENO,"ID not found\n",13);
					http_resp_code = kHttp404;
				}
			} else { // ID invalid
				http_resp_code = kHttp404;
				write(STDERR_FILENO,"No valid slotId\n",16);
			}
			break;
		default: // Todavia no se implementa
			write(STDERR_FILENO,"Method Not implemented yet\n",21);
			http_resp_code = kHttp501;
	}

	//char serializedResult[] = "#:10001|a:0|c:HardCoded1|f:7|o:56|t:21:33";
	char serializedResult[256] = {'\0'};
	if ( -1 == serialize(*result,serializedResult) ) {
		write(STDERR_FILENO,"something went wrong when serializing\n",34);
		http_resp_code = kHttp500;
	}
	http_response *rpn = (http_response *)malloc(sizeof(http_response));
	if ( 0 > response(http_resp_code, serializedResult, &rpn) ) {
		write(STDERR_FILENO,"response error\n",15);
		close(sd);
		free(req);
		free(rpn);
		pthread_exit(NULL);
	}

	puts("Luego de crear el response");
	if (debug) {
		printf("Response\n\tCODE: %d\n\tSTATUS: %s\n",rpn->code, rpn->statusM);
		if (rpn->body) printf("Response - BO: %s\n",rpn->body);
	}
	
	puts("a punto de llamar a dispatchResponse");
	if ( -1 == dispatchResponse(sd,rpn) ) {
		write(STDERR_FILENO,"no se pudo enviar la respuesta\n",31);
		free(req->body);
		free(req);
		free(rpn);
		free(result);
		close(sd);
	}

	if ( -1 == close(sd) ) { perror("close socket desc"); }

	puts("entro a la logica de la cola de mensajes");
	/* ---------------------------------- Message Queue --------------------------------------- */
	/* using mq_timesend because if the queue is full we don't want to block the thread for a long period of time */
	if (0 < mq) {
		struct timespec waitTime = {0};
		waitTime.tv_sec = 1; 	/* seconds */
		waitTime.tv_nsec = 0; 	/* nano seconds */
		if ( -1 == mq_timedsend(mq, "mensaje desde el hilo", 21, 0, &waitTime) ) {
			perror("mq_timesend");
		}
	}

	/* --------------------------------- Cleanup -------------------------------------------- */
	free(req->body);
	free(req);
	free(rpn);
	free(result);
	pthread_exit(NULL);
}
