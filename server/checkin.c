#include "app.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#define debug 0

/*
 * El resultado podria ser un puntero al nodo que se reserva
 * sinembargo, de esta manera, si otro hilo, modifica en algun otro momento
 * fuera del mutex a tal nodo. Cuando el programa quiera usar el puntero
 * ya sea para enviarlo como respuesta al cliente o algun otro motivo
 * ya va a ser inconsistente. Por ello, result deberia ser una variable
 * perteneciente SOLAMENTE al hilo que lo ejecute, y de esa manera garantizamos
 * la integridad de el resultado
 */
int checkin(struct slot *start, struct slot **result, pthread_mutex_t *mutex) {

	if (debug) printf("Entro a checkin con un slot desde el cliente: %s\n",(*result)->idCli);

	time_t rawtime;
	struct slot *node = start;	
	if ( 0 != pthread_mutex_lock(mutex) ) {
		perror("pthread_mutex_lock");
		return -1;
	}
	do {
		if (debug) printf("Nodo:%li idCli:%s Disp:%d hilo:%li\n", node->id, node->idCli, node->available, pthread_self());
		if (true == node->available) {
			time(&rawtime);
			node->available 			= 	false;
			strcpy(node->idCli,(*result)->idCli);
			node->checkInTime 			= 	localtime(&rawtime);
			//node->checkOutTime 			= 	{'\0'};
			memset(node->bill,'0',sizeof(node->bill));
			/*
			(*result)->id 				=	node->id;
			(*result)->available 		= 	false;
			(*result)->idCli 			= 	node->idCli;
			(*result)->floor 			= 	node->floor;
			(*result)->offset 			= 	node->offset;
			(*result)->checkInTime 		= 	node->checkInTime;
			(*result)->checkOutTime 	= 	node->checkOutTime;
			strcpy((*result)->bill,node->bill);
			*/
			//if (debug) printf("Asignando Nodo:%li idCli:%s hora:%s hilo:%li\n",(*result)->id, (*result)->idCli, asctime(node->checkInTime), pthread_self());
			break;
		} else {
			*result = node = node->next;
		}
	}while(node != NULL);
	if ( 0 != pthread_mutex_unlock(mutex) ) {
		//no se puede liberar el mutex, error grave, podria terminar bloqueando a todo el server
		perror("pthread_mutex_unlock");
		return -1;
	}
	
	return 0;
}
