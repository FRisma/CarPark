
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 1

int main (int argc, char *const argv[]) {

	puts("Trabajo Final - Computacion II - CarPark Client");

	// Manejo de opciones
	configuration *conf = (configuration *)malloc(sizeof(configuration));
	optionsHandler(argc, argv, conf);

	// Abro conexion con el socket
	
	// Muero
	
	return 0;
}
