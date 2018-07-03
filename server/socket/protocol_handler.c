#include "mySocket.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define debug 0

int protocol_handler(int protoId,int *sd,char *port){

	if (debug) printf("Protocol: %d - Port: %s\n",protoId,port);

	if (atoi(port) <= 1023) {
		perror("El puerto es invalido, debe ser mayor a 1023");
		return -1;
	}
	
	switch(protoId){
			case 4:  //IPV4
				if(debug) puts("Iniciando un socket de la familia ipv4");
				if(ipv4(sd,port) < 0) return -1;
				break;
			case 6:  //IPV6
				if(debug) puts("Iniciando un socket de la familia ipv6");
				if (ipv6(sd,port) < 0) return -1;
				break;
			default:
				if(debug) puts("Iniciando un socket del tipo unspec");
				if(unspec(sd,port) < 0) return -1;
	}
	return 0;
}
