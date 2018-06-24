#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define debug 0

long int slotIdFromURI(char *uri) {

	char *result, *ptr;
	result = strtok_r(uri,"/",&ptr);
	result = strtok_r(NULL,"/",&ptr);
	if (debug) printf("El slot es: %s\n",result);

	/* Check if all of the chars in result are digits. Move to a function */
	if (result != NULL) {
		size_t i=0;
		for (i=0;i<strlen(result);i++) {
			if (!isdigit(result[i])) {
				return -1;
			}
		}
		return atol(result);
	} else {
		return -1;
	}

}
