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

    // Argument handling
    opterr=1;   // Disables getopt() error printing.
	while ((opt = getopt(argc, argv, "cd:s:h")) >= 0 ){ // -c create -d delete -s status -h help
		switch (opt){
			case 'c':
				if (debug) printf("Create a ticket\n");
				conf->opt = 'c';
				break;
			case 'd':
				if (debug) printf("Delete ticket\n");
				conf->opt = 'd';
				if (strlen(optarg) == SLOT_ID_LENGHT) {
					if (debug) printf("Ticket:%s\n",optarg);
					strncpy(conf->slotId, optarg, strnlen(optarg,SLOT_ID_LENGHT)+1);
				}
				break;
			case 's':
				if (debug) printf("Status\n");
				conf->opt = 's';
				if (strlen(optarg) == SLOT_ID_LENGHT) {
					if (debug) printf("Ticket:%s\n",optarg);
					strncpy(conf->slotId, optarg, strnlen(optarg,SLOT_ID_LENGHT)+1);
				}
				break;
			case 'h':
			case '?':
				puts("-c ask for a creation of a new ticket -d ask for a ticket number to be finished -s status of a ticket -h help");
                exit(EXIT_SUCCESS);
			default:
				puts("Sintaxis ambigua. Intente ejecutar con -h para mas detalles");
                exit(EXIT_SUCCESS);
		}
	}
}
