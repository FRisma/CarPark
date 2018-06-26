#include "app.h"

#include <stdio.h>

void printLocation(slot *s) {

	char displayInTime[50] = {'\0'};
	asctime_r(s->checkInTime,displayInTime);
	char displayOutTime[50] = {'\0'};
	asctime_r(s->checkOutTime,displayOutTime);

	printf("Nodo id:%lu avail:%d idCli:%s floor:%d offset:%lu tIn:%s tOut:%s bill:%s\n",\
			s->id,s->available,s->idCli,s->floor,s->offset,displayInTime,displayOutTime,s->bill); 
}
