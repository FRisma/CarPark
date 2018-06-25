#include "app.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define debug 0

int checkout(struct slot *start, struct slot *result, pthread_mutex_t *mutex) {

	struct slot *node = start;
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pther_mutex_lock");
		return -1;
	}
	do {
		if ( result->id == node->id && node->available == false) {
			time_t *rawtime = (time_t *)malloc(sizeof(time_t));
			time(rawtime);
			// Maping to result
			result->id 				=	node->id;
			result->available 		= 	false;
			result->floor 			= 	node->floor;
			result->offset 			= 	node->offset;
			result->checkInTime 	= 	node->checkInTime;
			result->checkOutTime 	= 	localtime(rawtime);
			strcpy(result->idCli,node->idCli);
			strcpy(result->bill,node->bill);
			// Make the node available again
			node->available 		= 	true;
			node->checkInTime		= 	NULL;
			node->checkOutTime		= 	NULL;
			memset(node->bill,'\0',16*sizeof(char));
			memset(node->idCli,'\0',10*sizeof(char));
			if (debug) { puts("Checkout:"); printLocation(result);}
			break;
		} else {
			node = node->next;
			result->id = -1;
		}
	}while(node != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex
		perror("pthread_mutex_unlock");
		return -1;
	}
	return 0;
}	
