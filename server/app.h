#ifndef APP_H_
	#define APP_H_

	#include "macros.h"
	#include "utils/utils.h"
	#include <mqueue.h>

	// Estructura de una posicion en un estacionamiento
	typedef struct pos {
		long int id;
		int floor;
		unsigned int offset;
		unsigned long checkInTime;
	}slot;

	// Estructura de configuracion del server
	typedef struct params {
		int protocol;
		int socketDescriptor;
		char mqName[MAX_MQ_NAME_LENGTH];
		char port[MAX_PORT_LENGTH];
	}serverConf;

	// Estructura de datos para los hilos
	typedef struct data {
		char *port;
		int csd; //Socket descriptor
		mqd_t mqsd; //Message queue descriptor
	}threadData;

	// Esta es la funcion inicial que crea el contexto necesario para el funcionamiento, crea la cola de mensajes,
	// obtiene la configuracion necesaria para el server del archivo de configuracion
	// crea el socket, crea la cola de mensajes
	int parkingManager(char *configFile);

	// Abre el archivo de configuracion y parsea la informacion para escribir el resultado en port y mqName
	int configServer(char *cfgfile, char *port, char *mqName);

	// Se crea el contexto necesario para el funcionamiento de los hilos, se pone el socket a escuchar y finalmente ante
	// cada nueva conexion al socket, se llama a la funcion que atiende a cada cliente.
	int runServer(serverConf *conf);

	// Funcion que ejecuta cada hilo por cada nueva conexion
	int threadWork(threadData *data);

#endif
