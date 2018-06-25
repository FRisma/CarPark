#include "app.h"

#include <stdio.h>

void listLocations(slot *start) {
	
	struct slot *tmp = start;
	do {
		printLocation(tmp);
		tmp = tmp->next;
	}while(tmp != NULL);
}

