#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int createLocations(slot **startingNode) {

	struct slot *tmp = NULL;
	int nodes = 10;
	int loop = 0;
	
	for (loop = 0; loop < nodes; loop++) {
		slot *newSlot = NULL;
		if ( NULL == (newSlot = (slot *)malloc(sizeof(slot))) ) {
			perror("malloc");
			return -1;
		}
		newSlot->id 			= loop+10000;
		newSlot->available 		= true;
		newSlot->floor 			= 0;
		newSlot->offset 		= loop;
		newSlot->checkInTime 	= NULL;
		newSlot->checkOutTime 	= NULL;
		memset(newSlot->bill,'\0',16*sizeof(char));
		memset(newSlot->idCli,'\0',10*sizeof(char));
		strncpy(newSlot->bill,"$0",2);
		strncpy(newSlot->idCli,"-1",2);
		newSlot->next 			= NULL;
		if (debug) printf("Creo el nodo con el id: %lu\n",newSlot->id);
		if (0 == loop) {
			*startingNode = newSlot;
		}
		if (NULL != tmp) {
			tmp->next = newSlot;
		}
		tmp = newSlot;
	}
	return 0;
}
