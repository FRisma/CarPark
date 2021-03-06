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
		char protocol[2];
		int socketDescriptor;
		char mqName[MAX_MQ_NAME_LENGTH];
		char port[MAX_PORT_LENGTH];
		mqd_t mqd; //Message queue descriptor
	}serverConf;

	// Estructura de datos para los hilos
	typedef struct {
		int csd;				 /* Socket descriptor */
		mqd_t mqd; 				 /* Message queue descriptor */
		pthread_mutex_t *sincro; /* Mutex for managing concurrency when accessing the linked list */
		slot *start; 			 /* Points to the begining of a linkedlist allocated in the heap to be shared by the threads */
	}threadData;

	/* Esta es la funcion inicial que crea el contexto necesario para el funcionamiento, crea la cola de mensajes,
	 * obtiene la configuracion necesaria para el server del archivo de configuracion
	 * crea el socket, crea la cola de mensajes
	 */
	int parkingManager(char *configFile);

	/* Abre el archivo de configuracion y parsea la informacion para escribir el resultado en port y mqName */
	int configServer(char *cfgfile, char *protocol, char *port, char *mqName);

	/* Create a linked list for the slots */
	int createLocations(slot **startingNode);
	
	/* Frees the memory allocated by createLocations */
	int freeLocations(slot *startingNode);

	/* Prints the complete node list */
	void listLocations(slot *start);
	
	/* Prints in a friendly way a single slot */
	void printLocation(slot *s);

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
	int checkin(slot *startingNode, slot *result, pthread_mutex_t *mutex);

	/* Checkout, frees the reservation matching the result->id */
	int checkout(slot *startingNode, slot *result, pthread_mutex_t *mutex);

	/* Will return the current state for a reservation indicated by slotId and the result will be placed in result */
	int status(slot *startingNode, slot *result, long int slotId);

	/* Performs the billing taking into consideration the checkInTime and the currentTime.
	 * The result will be stored in the same slot sructure */
	int billing(slot *current);

	/* Sends the response to the descriptor identified by sd*/
	int dispatchResponse(int sd, http_response resp);

	/* Send a message to que queue described by messageQueue. Mode specifies if
	 * it is possible that the delivery is tolerant to failure. If mode is 1, it
	 * will fail if it can't send the message. If mode is 0, will try to send
	 * the message only for a period of time.*/
	int logActivity(mqd_t messageQueue, http_request req, int mode);

#endif
