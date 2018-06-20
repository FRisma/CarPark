#include "app.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define debug 1

int checkin(struct slot *start, struct slot **result, pthread_mutex_t *mutex) {

	time_t rawtime;
	struct slot *node = start;	
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pther_mutex_lock");
		return -1;
	}
	do {
		//if (debug) printf("Nodo: %li Disp:%d hilo:%li\n", result->id, result->available, pthread_self());
		if (true == node->available) {
			printf("Asignando Nodo: %li hilo:%li\n", node->id, pthread_self());
			node->available = false;
			node->idCli = 345;
			time(&rawtime);
			(*result)->checkInTime = localtime(&rawtime);
			break;
		} else {
			*result = (*result)->next;
		}
	}while(result != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex
		perror("pthread_mutex_unlock");
		return -1;
	}
	
	return 0;
}
