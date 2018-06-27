#include "app.h"

#include <stdio.h>
#include <string.h>

void printLocation(slot *s) {

	char displayInTime[50] = {'\0'};
	if (NULL != s->checkInTime) {strftime(displayInTime, sizeof(displayInTime), "%c", s->checkInTime);}
	else { strncpy(displayInTime,"00:00",5);}
	char displayOutTime[50] = {'\0'};
	if (NULL != s->checkOutTime) {strftime(displayOutTime, sizeof(displayOutTime), "%c", s->checkOutTime); }
	else { strncpy(displayOutTime,"00:00",5);}

	printf("Nodo id:%li avail:%d idCli:%s floor:%d offset:%lu tIn:%s tOut:%s bill:%s\n",\
			s->id,s->available,s->idCli,s->floor,s->offset,displayInTime,displayOutTime,s->bill); 
}
