#include "app.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define debug 1

slot* checkin(struct slot *start, pthread_mutex_t *mutex) {

	struct slot *node = start;
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pther_mutex_lock");
		return -1;
	}
	do {
		//if (debug) printf("Nodo: %li Disp:%d hilo:%li\n", node->id, node->available, pthread_self());
		if (true == node->available) {
			printf("Asignando Nodo: %li hilo:%li\n", node->id, pthread_self());
			node->available = false;
			node->idCli = 678;
			node->checkInTime = 23.32;
			break;
		} else {
			node = node->next;
		}
	}while(node != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex
		perror("pthread_mutex_unlock");
		return NULL;
	}
	
	// Si no hay posiciones libres, node sale siendo NULL
	if (node == NULL) {
		//Todo lleno
	}

	return node;
}
