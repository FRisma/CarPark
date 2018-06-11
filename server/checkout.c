#import "app.h"

int checkout(struct slot *current, struct const slot *start) {

	struct slot *node = start;
	do {
		printf("T Nodo: %li\n", node->id);
		if (current->id == node->id) {
			node->available = true;
			node->idCli = 0;
			//Reset timer
			node->checkInTime = 0;
			break;
		} else {
			node = node->next;
		}
	}while(node != NULL);

	return 0;
}	
