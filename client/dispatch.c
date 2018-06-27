#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define debug 0

int dispatch(int sd, char *req, char *resp) {

	char buff[2*1024] = {'\0'};
	int leido = 0;

	if (debug) printf("El request es: %s\n",req);
	if ( -1 == write(sd, req, strlen(req)) ) {
		perror("write");
		return -1;
	}
	while( 0 < (leido = read(sd, buff, sizeof(buff))) ) {
		if (-1 == leido) {
			perror("read");
			return -1;
		}
		strncpy(resp,buff,strnlen(buff,sizeof(buff)));
		if (debug) write(STDOUT_FILENO,buff,leido);
		if (debug) write(STDOUT_FILENO,"\n",1);
	}

	return 0;
}
