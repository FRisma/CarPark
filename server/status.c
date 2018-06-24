#include "app.h"

#include <stdio.h>
#include <string.h>

#define debug 0

int status(struct slot *start, struct slot **result, long int nodeId) {

	// Check si result tiene memoria reservada, sino salir
	if (debug) printf("status for node %li\n",nodeId);
	struct slot *node = start;	
	do {
		if (node->id == nodeId) {
			if (debug) printf("Encontre el nodo para preguntar por su status %li\n",node->id);
			// COPIAR (no usar un putero) COPIAR el estado de este nodo
			// a otro nodo en el heap asi no rompemos la lista original cuando
			// liberemos el nodo resultado
			(*result)->id 				=	node->id;
			(*result)->available 		= 	node->available;
			(*result)->floor 			= 	node->floor;
			(*result)->offset 			= 	node->offset;
			(*result)->checkInTime 		= 	node->checkInTime;
			(*result)->checkOutTime 	= 	node->checkOutTime;
			strcpy((*result)->idCli,node->idCli);
			strcpy((*result)->bill,node->bill);
			break;
		} else {
			node = node->next;
			(*result)->id = -1;
		}
	}while(node != NULL);

	return 0;
}
