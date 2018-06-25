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
	struct slot *start = arg->start;
	free(data); //liberar esta memoria, no tiene problemas porque los datos de la misma ya fueron copiados y los punteros fueron referenciados por otros
	
	if (debug) {
		printf("ThreadArgs->connectionSD:%d threadArgs->mqd:%d\n",sd, mq);
		listLocations(start);
	}

	/* ------------------- Request Handing ------------------------- */
	http_request *req = (http_request *)malloc(sizeof(http_request));
	req->body = NULL;
	if ( 0 > requestHeader(sd,req)) {
		write(STDERR_FILENO,"Error: requestHeader\n",21);
		if ( -1 == close(sd) ) { perror("close socket desc"); }
		free(req->body);
		free(req);
		pthread_exit(NULL);
	}

	if (debug) {
		printf("Request\n\tMT: %d\n\tRS: %s\n\t",req->method, req->resource);
		if (NULL != req->body && strlen(req->body) > 0){
			printf("CT: %s\n\tCL: %li\n\tBO: %s\n", req->content_type, req->content_length, req->body);
		}
	}

	/* ------------------- Response Handling ------------------------- */
	int http_resp_code = 0;
	struct slot *result = NULL;
	if ( NULL == (result=(slot*)malloc(sizeof(slot))) ) { // Pointer to the result
		perror("malloc");
		if ( -1 == close(sd) ) { perror("close socket desc"); }
		free(req->body);
		free(req);
		pthread_exit(NULL);
	}
	memset(result,'\0',1*sizeof(result));
	
	switch (req->method) {
		case POST:
			if (-1 != deserialize(req->body, &result)) { // Map body to slot
				if ( -1 != checkin(start,result,mutex) ) {
					if (0 > result->id) {
						write(STDERR_FILENO,"No more places\n",15);
						http_resp_code = kHttp204;
					} else {
						if (debug) {
							puts("Asignando nodo");
							printLocation(result);
						}
						http_resp_code = kHttp201;
					}
				} else {
					http_resp_code = kHttp500;
					write(STDERR_FILENO,"Checkin error\n",14);
				}
			} else {
				http_resp_code = kHttp400;
				write(STDERR_FILENO,"No valid request body\n",22);
			}
			break;
		case GET:
			if (debug) printf("Status reserva: %s\n",req->resource);
			long int idGet = slotIdFromURI(req->resource);
			if (-1 != idGet) {
				if (-1 != status(start,result,idGet)) {
					if (0 < result->id) {
						billing(result);
						http_resp_code = kHttp200;
						if (debug) {
							puts("Status nodo");
							printLocation(result);
						}
					} else { // ID not found
						write(STDERR_FILENO,"ID not found\n",13);
						http_resp_code = kHttp404;
					}
				} else {
					http_resp_code = kHttp500;
					write(STDERR_FILENO,"Status error\n",13);
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
				result->id = idDelete;
				if ( -1 != checkout(start, result, mutex) ) {
					if (0 < result->id) {
						billing(result);
						http_resp_code = kHttp200;
						if (debug) {
							puts("Eliminando nodo");
							printLocation(result);
						}
					} else { // ID not found
						write(STDERR_FILENO,"ID not found\n",13);
						http_resp_code = kHttp404;
					}
				} else {
					http_resp_code = kHttp500;
					write(STDERR_FILENO,"Checkout error\n",15);
				}
			} else { // ID invalid
				http_resp_code = kHttp400;
				write(STDERR_FILENO,"No valid slotId\n",16);
			}
			break;
		default: // Todavia no se implementa
			write(STDERR_FILENO,"Method Not implemented yet\n",21);
			http_resp_code = kHttp501;
	}

	char serializedResult[256] = {'\0'};
	if (debug) { puts("Mando a serializar:"); printLocation(result); }
	if ( -1 == serialize(*result,serializedResult) ) {
		write(STDERR_FILENO,"something went wrong when serializing\n",34);
		http_resp_code = kHttp500;
	}
	http_response *rpn = (http_response *)malloc(sizeof(http_response));
	if ( 0 > response(http_resp_code, serializedResult, rpn) ) {
		write(STDERR_FILENO,"response error\n",15);
	}

	if (debug) {
		printf("Response\n\tCODE: %d\n\tSTATUS: %s\n",rpn->code, rpn->statusM);
		if (rpn->body) printf("Response - BO: %s\n",rpn->body);
	}
	
	if ( -1 == dispatchResponse(sd,*rpn) ) {
		write(STDERR_FILENO,"no se pudo enviar la respuesta\n",31);
	}

	if ( -1 == close(sd) ) { perror("close socket desc"); }

	/* ---------------------------------- Message Queue --------------------------------------- */
	if ( -1 == logActivity(mq,*req,1) ) {
		write(STDERR_FILENO,"Error logAtivity\n",17);
	}
	
	/* --------------------------------- Cleanup -------------------------------------------- */
	puts("Cleaning out & leaving");
	free(req->body);
	free(req);
	free(rpn);
	//free(result->checkInTime);
	//free(result->checkOutTime);
	free(result);
	pthread_exit(NULL);
}
