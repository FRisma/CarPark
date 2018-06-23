#include "app.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

char * const kPattern = "id:%li|a:%d|idC:%li|f:%d|o:%lu";

#define debug 1

int serialize(slot s, char *serialized) {

	int d_id 		= floor(log10(abs(s.idCli))) + 1;
	int d_available = floor(log10(abs(s.idCli))) + 1;
	int d_idCli 	= floor(log10(abs(s.idCli))) + 1;
	int d_floor 	= floor(log10(abs(s.idCli))) + 1;
	int d_offset 	= floor(log10(abs(s.idCli))) + 1;
	//if ( 0 > snprintf(serialized, idDigits+1,"%li",s.idCli) ) {
	if ( 0 > snprintf(serialized, d_id + d_available + d_idCli + d_floor + d_offset + 18 ,kPattern, s.id, s.available, s.idCli, s.floor, s.offset) ) {
		write(STDERR_FILENO,"serialize snprintf",18);
		return -1;
	}

	if (debug) printf("Serialize Resultado %s\n",serialized);
	return 0;
}
