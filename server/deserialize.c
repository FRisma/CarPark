#include "app.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define debug 0

/* Headers */
int parseToken(char *token, slot **result);
int deserialize(char *stream, slot **result);

/* Implementation */
int deserialize(char *stream, slot **result) {

	char *value, *ptr = NULL;
	value = strtok_r(stream,"|",&ptr);
	if (debug) printf("El valor es: %s\n",value);
	parseToken(value, result);

	while( NULL != (value=strtok_r(NULL,"|",&ptr)) ) {
		if (debug) printf("Valor %s\n", value);
		parseToken(value, result);
	}

	return 0;
}

int parseToken(char *token, slot **result) {
	char *key, *value, *ptr = NULL;
	key 	= strtok_r(token,":",&ptr);
	value 	= strtok_r(NULL,":",&ptr);

	if (key == NULL || value == NULL) {
		puts("No se encontro una key o un value valido para el token");
		return -1;
	}
	if (debug) printf("key: %s value: %s\n",key,value);
	switch((char)*key) {
		case '#':
			(*result)->id = atol(value);
			break;
		case 'a':
			(*result)->available = atoi(value);
			break;
		case 'c':
			strcpy((*result)->idCli,value);
			break;
		case 'f':
			(*result)->floor = atoi(value);
			break;
		case 'o':
			(*result)->offset = atol(value);
			break;
	}

	return 0;
}
