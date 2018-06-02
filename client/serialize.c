#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int serialize(slot s, char *serialized) {

	if ( 0 > snprintf(serialized,256,"{%li%d%d}",s.idCli,s.floor,s.offset) ) {
		write(STDERR_FILENO,"serialize snprintf",18);
		return -1;
	}

	return 0;
}
