#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define debug 1

/*
 * El resultado podria ser un puntero al nodo que se reserva
 * sinembargo, de esta manera, si otro hilo, modifica en algun otro momento
 * fuera del mutex a tal nodo. Cuando el programa quiera usar el puntero
 * ya sea para enviarlo como respuesta al cliente o algun otro motivo
 * ya va a ser inconsistente. Por ello, result deberia ser una variable
 * perteneciente SOLAMENTE al hilo que lo ejecute, y de esa manera garantizamos
 * la integridad de el resultado
 */
int checkin(struct slot *start, struct slot *result, pthread_mutex_t *mutex) {

	if (debug) printf("Entro a checkin con un slot desde el cliente: %s\n",result->idCli);

	struct slot *node = start;	
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pthread_mutex_lock");
		return -1;
	}
	do {
		if (true == node->available) {
			time_t *rawtime = (time_t *)malloc(sizeof(time_t));
			time(rawtime);
			//localtime_r(rawtime,result->checkInTime);
			node->checkInTime 		= 	localtime(rawtime);
			node->available 		= 	false;
			node->checkOutTime 		= 	NULL;
			strncpy(node->idCli,result->idCli,strlen(result->idCli));
			// Maping to result
			result->id 				=	node->id;
			result->available 		= 	false;
			result->floor 			= 	node->floor;
			result->offset 			= 	node->offset;
			result->checkInTime 	= 	node->checkInTime;
			result->checkOutTime 	= 	NULL;
			strcpy(result->idCli,node->idCli);
			strcpy(result->bill,node->bill);
			if (debug) { puts("checkin node:"); printLocation(result); }
			break;
		} else {
			node = node->next;
			result->id = -1;
		}
	}while(node != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex, error grave, podria terminar bloqueando a todo el server
		perror("pthread_mutex_unlock");
		return -1;
	}
	
	return 0;
}
