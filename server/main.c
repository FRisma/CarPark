#include "app.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 1

int main (int argc, char *const argv[]) {

	puts("Trabajo Final - Computacion II - CarPark");
	
	// Manejar opciones
  	configuration *conf = (configuration *)malloc(sizeof(configuration));
	optionsHandler(argc, argv, conf);
	if (debug) printf("Parameters: ParkServer %s\n", conf->parkingServerCfgFile);

	// Lanzar el server de estacionamiento
	parkingManager(conf->parkingServerCfgFile);
	
	return 0;
}
