#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define debug 1

int dispatch(int sd, char *req, char *resp) {

	puts("Dentro de request");
	//En realidad deberia recibir un char* para escribir la respuesta y usar el
	//return value para control de errores.
	//de esa manera, no necesito crear buff
	char buff[512];
	int leido;

	if (debug) printf("El request es: %s\n",req);
	puts("A punto de mandarlo por el socket");
	if ( -1 == write(sd, req, strlen(req)) ) {
		perror("write");
		return -1;
	}
	while( 0 < (leido = read(sd, buff, sizeof(buff))) ) {
		if (-1 == leido) {
			perror("read");
			return -1;
		}
		if (debug) write(STDOUT_FILENO,buff,leido);
		//algun mecanismo para realloc del puntero y appendearle la siguiente lectura
	}

	return 0;
}