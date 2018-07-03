#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define debug 1

long int slotIdFromURI(char *uri) {

	if(debug) printf("slotFromURI la URI es %s\n",uri);
	char *result, *ptr;
	result = strtok_r(uri,"/",&ptr);
	result = strtok_r(NULL,"/",&ptr);
	if (debug) printf("El slot es: %s\n",result);

	/* Check if all of the chars in result are digits. Move to a function */
	if (result != NULL) {
		size_t i=0;
		for (i=0;i<strlen(result);i++) {
			if (!isdigit(result[i])) {
				puts("No digit found");
				return -1;
			}
		}
		puts("Digit found");
		return atol(result);
	} else {
		puts("Bad request");
		return -1;
	}

}
