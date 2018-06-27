#include "app.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int main (int argc, char *const argv[]) {

	puts("\t\t ..:: Trabajo Final - Computacion II - CarPark ::..");
	
	// Manejar opciones
  	configuration *conf = NULL;
	if ( NULL == (conf=(configuration *)malloc(sizeof(configuration))) ) {
			perror("malloc");
			return -1;
	}
	memset(conf,'\0',sizeof(configuration));
	memset(conf->parkingServerCfgFile,'\0',FILE_PATH_LENGTH);
	if (conf) {
		optionsHandler(argc, argv, conf);
		if (debug) printf("Parameters: ParkServer %s\n", conf->parkingServerCfgFile);
	
		// Lanzar el server de estacionamiento
		parkingManager(conf->parkingServerCfgFile);
	} else {
		perror("malloc");
		return -1;
	}
	free(conf);
	return 0;
}
