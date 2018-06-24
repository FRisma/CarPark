#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int createLocations(slot **startingNode) {

	struct slot *tmp;
	int nodes = 100;
	int loop = 0;
	
	for (loop = 0; loop < nodes; loop++) {
		slot *newSlot = (slot *)malloc(sizeof(slot));
		newSlot->id = loop+10000;
		newSlot->available = true;
		newSlot->floor = 0;
		newSlot->offset = loop;
		newSlot->checkInTime = NULL;
		newSlot->checkOutTime = NULL;
		//*(newSlot->checkInTime) = {0};
		//*(newSlot->checkOutTime) = {0};
		//memset((*newSlot->checkInTime),'0',sizeof(struct tm));
		//memset(*(newSlot->checkOutTime),'0',sizeof(struct tm));
		memset(newSlot->bill,'\0',16*sizeof(char));
		memset(newSlot->idCli,'\0',10*sizeof(char));
		newSlot->next = NULL;
		if (debug) printf("Creo el nodo con el id: %lu\n",newSlot->id);
		if (0 == loop) {
			*startingNode = newSlot;
		}
		if (tmp) {
			tmp->next = newSlot;
		}
		tmp = newSlot;
	}

	
	free(tmp);
	return 0;
}
