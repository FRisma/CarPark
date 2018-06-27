#include "app.h"

#include <stdio.h>
#include <string.h>

#define debug 0

int status(struct slot *start, struct slot *result, long int nodeId) {

	// Check si result tiene memoria reservada, sino salir
	//if (debug) printf("status for node %li\n",nodeId);
	struct slot *node = start;	
	do {
		if (node->id == nodeId) {
			result->id 				=	node->id;
			result->available 		= 	node->available;
			result->floor 			= 	node->floor;
			result->offset 			= 	node->offset;
			result->checkInTime 	= 	node->checkInTime;
			result->checkOutTime 	= 	node->checkOutTime;
			strcpy(result->idCli,node->idCli);
			strcpy(result->bill,node->bill);
			if (debug) { puts("Status"); printLocation(result); }
			break;
		} else {
			node = node->next;
			result->id = -1;
		}
	}while(node != NULL);

	return 0;
}
