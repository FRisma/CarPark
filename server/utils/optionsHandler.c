#include "optionsHandler.h"

#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define debug 0

void optionsHandler (int argc, char* const argv[], configuration *conf) {

    int opt = 0;

	// Configure default values
	strncpy(conf->parkingServerCfgFile, DEFAULT_PARK_CONFIGFILE, strnlen(DEFAULT_PARK_CONFIGFILE,FILE_PATH_LENGHT)+1);

    // Argument handling
    opterr=1;   // Disables getopt() error printing.
	while ((opt = getopt(argc, argv, "p:h")) >= 0 ){ // -p parkingServerConfigFile -h help
		switch (opt){
			case 'p':
				if (debug) printf("ParkConfig file: %s\n", optarg);
				strncpy(conf->parkingServerCfgFile, optarg, strnlen(optarg,FILE_PATH_LENGHT)+1);
				break;
			case 'h':
			case '?':
				puts("-w webServerFile.cfg -p parkingServer.cfg");
                exit(EXIT_SUCCESS);
			default:
				puts("Sintaxis ambigua. Intente ejecutar con -h para mas detalles");
                exit(EXIT_SUCCESS);
		}
	}
}
