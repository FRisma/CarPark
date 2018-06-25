#include "app.h"

#include <stdio.h>

void printLocation(slot *s) {

	printf("Nodo id:%lu avail:%d idCli:%s floor:%d offset:%lu tIn:%s tOut:%s bill:%s\n",\
			s->id,s->available,s->idCli,s->floor,s->offset,asctime(s->checkInTime),asctime(s->checkOutTime),s->bill); 
}
