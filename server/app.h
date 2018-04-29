#ifndef APP_H_
	#define APP_H_

	#include "utils/utils.h"

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
		char port[1024];
	}serverConf;

	// Estructura de datos para los hilos
	typedef struct data {
		char *port;
		int csd;
	}threadData;

	int parkingManager(char *configFile);

	int configServer(char *configFile, char *port);

	int runServer(serverConf *conf);

	int threadWork(threadData *data);

#endif
