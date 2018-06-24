#ifndef APP_H_
	#define APP_H_

	#include "macros.h"
	#include "utils/utils.h"
	#include "requestHeader.h"
	#include "response.h"
	#include <mqueue.h>
	#include <pthread.h>
	#include <stddef.h>

	typedef enum {
		false,
		true
	}bool;

	// Estructura de una posicion en un estacionamiento
	typedef struct slot{
		long int id;
		bool available;
		char idCli [10];
		int floor;
		size_t offset;
		struct tm *checkInTime;
		struct tm *checkOutTime;
		char bill [16];
		struct slot *next;
	}slot;

	// Estructura de configuracion del server
	typedef struct {
		int protocol;
		int socketDescriptor;
		char mqName[MAX_MQ_NAME_LENGTH];
		char port[MAX_PORT_LENGTH];
		mqd_t mqd; //Message queue descriptor
	}serverConf;

	// Estructura de datos para los hilos
	typedef struct {
		int csd;				/* Socket descriptor */
		mqd_t mqd; 				/* Message queue descriptor */
		pthread_mutex_t *sincro; /* Mutex for managing concurrency when accessing the linked list */
		slot *start; 			/* Points to the begining of a linkedlist allocated in the heap to be shared by the threads */
	}threadData;

	/* Esta es la funcion inicial que crea el contexto necesario para el funcionamiento, crea la cola de mensajes,
	 * obtiene la configuracion necesaria para el server del archivo de configuracion
	 * crea el socket, crea la cola de mensajes
	 */
	int parkingManager(char *configFile);

	/* Abre el archivo de configuracion y parsea la informacion para escribir el resultado en port y mqName */
	int configServer(char *cfgfile, char *port, char *mqName);

	/* Launches the logging process on a different process, this will fork.
	 * The arg will be the name of the message queue the new process will try to open
	 */
	int startLoggingProcess(char *arg);

	/* Create a linked list for the slots */
	int createLocations(slot **startingNode);
	
	/* Frees the memory allocated by createLocations */
	int freeLocations(slot *startingNode);

	/* Se crea el contexto necesario para el funcionamiento de los hilos, se pone el socket a escuchar y finalmente ante
	 * cada nueva conexion al socket, se llama a la funcion que atiende a cada cliente.
	 */
	int runServer(serverConf *conf);

	/* Funcion que ejecuta cada hilo por cada nueva conexion */
	void *threadWork(void *data);

	/* Will convert a slot into a stram of bytes */
	int serialize(slot result, char *stream);

	/* Will convert a stream of bytes into a slot */
	int deserialize(char *stream, slot **result);

	/* Will parse uri to  return a slotId as a char * for those requests that
	 * sends the id in the url instead of the body
	 */
	long int slotIdFromURI(char *uri);

	/* Checkin, looks for an available slot and makes a reservation, the new location is pointed by result */
	int checkin(slot *startingNode, slot **result, pthread_mutex_t *mutex);

	/* Checkout, frees the reservation matching the currentNode->id */
	int checkout(slot *startingNode, slot **currentNode, pthread_mutex_t *mutex);

	/* Will return the current state for a reservation indicated by slotId and the result will be placed in resutNode */
	int status(slot *startingNode, slot **resultNode, long int slotId);

	/* Performs the billing taking into consideration the checkInTime and the currentTime.
	 * The result will be stored in the same slot sructure */
	int billing(slot *current);

	/* Sends the response to the descriptor identified by sd*/
	int dispatchResponse(int sd, http_response *response);

#endif
