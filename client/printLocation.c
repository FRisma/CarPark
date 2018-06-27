#include "app.h"

#include <stdio.h>

void printLocation(slot *s) {

	printf("Nodo id:%lu\nDisponible:%d\nPuerta:%s\nPiso:%d\nPosicion:%lu\nHora entrada:%s\nHora salida:%s\nTotal a pagar:%s\n",\
			s->id,s->available,s->idCli,s->floor,s->offset,s->checkInTime,s->checkOutTime,s->bill);
}
