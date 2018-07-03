#include "app.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

/* Headers */
int parseToken(char *token, slot *result);
int deserialize(char *stream, slot *result);

/* Implementation */
int deserialize(char *stream, slot *result) {

	char *value, *ptr = NULL;
	char aux[512] = {'\0'};
	//Body parsing
	if ( parse("\r\n\r\n.*",stream,4,aux) < 0) {
		write(STDERR_FILENO,"no body\n",8);
		return -1;
	}
	if (strnlen(aux,512) > 0) {
		value = strtok_r(aux,"|",&ptr);
		parseToken(value, result);

		while( NULL != (value=strtok_r(NULL,"|",&ptr)) ) {
			parseToken(value, result);
		}
	}
	return 0;
}

int parseToken(char *token, slot *result) {
	char *key, *value, *ptr = NULL;
	key 	= strtok_r(token,"-",&ptr);
	value 	= strtok_r(NULL,"-",&ptr);

	if (key == NULL || value == NULL) {
		puts("No se encontro una key o un value valido para el token");
		return -1;
	}
	if (debug) printf("key: %s value: %s\n",key,value);
	switch((char)*key) {
		case '#':
			result->id = atol(value);
			break;
		case 'a':
			result->available = atoi(value);
			break;
		case 'c':
			strcpy(result->idCli,value);
			break;
		case 'f':
			result->floor = atoi(value);
			break;
		case 'o':
			result->offset = atol(value);
			break;
		case 'i':
			strncpy(result->checkInTime,value,200);
			break;
		case 't':
			strncpy(result->checkOutTime,value,200);
			break;
		case 'b':
			strncpy(result->bill,value,16);
			break;
	}

	return 0;
}
