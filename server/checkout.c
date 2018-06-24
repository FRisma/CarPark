#include "app.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>

int checkout(struct slot *start, struct slot **currentNode, pthread_mutex_t *mutex) {

	time_t rawTime;
	struct slot *node = start;
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pther_mutex_lock");
		return -1;
	}
	do {
		printf("T Nodo: %li\n", node->id);
		if ( (*currentNode)->id == node->id) {
			node->checkOutTime = localtime(&rawTime);
			// COPIAR (no usar un putero) COPIAR el estado de este nodo
			// a otro nodo en el heap asi podemos liberar node
			break;
		} else {
			node = node->next;
		}
	}while(node != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex
		perror("pthread_mutex_unlock");
		return -1;
	}
	return 0;
}	
