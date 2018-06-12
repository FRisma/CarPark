#include "app.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define debug 0

int serialize(slot s, char *serialized) {

	int idDigits = floor(log10(abs(s.idCli))) + 1;
	if ( 0 > snprintf(serialized, idDigits+7,"{cli:%li}",s.idCli) ) {
		write(STDERR_FILENO,"serialize snprintf",18);
		return -1;
	}

	if (debug) printf("Dentro de serialize: idDigits:%d s.idCli:%li\nResultado %s\n",idDigits,s.idCli,serialized);
	return 0;
}
