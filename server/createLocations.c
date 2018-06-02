#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

int createLocations(slot **startingNode) {

	struct slot *tmp;
	int nodes = 100;
	int loop = 0;
	
	for (loop = 0; loop < nodes; loop++) {
		slot *newSlot = (slot *)malloc(sizeof(slot));
		newSlot->id = loop*10;
		newSlot->floor = 0;
		newSlot->offset = loop;
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
