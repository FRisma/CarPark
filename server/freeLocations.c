#include "app.h"

#include "stdlib.h"

int freeLocations (slot *start) {

	struct slot *tmp = NULL;
	while(start->next != NULL) {
		tmp = start;
		start = start->next;
		if (tmp != NULL) {
			free(tmp);
		}
	}
	return 0;
}
