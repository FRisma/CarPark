#include "mySocket.h"

#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#define debug 0

int unspec(int *sd,const char *puerto){
	
    if (debug) printf("\nUNSPEC.C\nPuerto:%s\n",puerto);

	struct addrinfo hints, *res, *ptr;
	int opc = 1;

	memset(&hints,0,sizeof(struct addrinfo));	
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  	hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_PASSIVE;

	int errcode;
	if ( (errcode = getaddrinfo(NULL,puerto,&hints,&res)) != 0 ){
		printf("Error unspec: %s\n", gai_strerror(errcode));
		return -1;
	}


	for ( ptr = res; ptr != NULL; ptr = ptr->ai_next ){
		*sd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (*sd < 0){
			perror("socket unspec");
			continue;
		}
		if (setsockopt(*sd,SOL_SOCKET,SO_REUSEADDR,&opc,sizeof opc) < 0){
			perror("setsockopt");
		}
		if (bind(*sd, ptr->ai_addr, ptr->ai_addrlen) == 0)
			return 0; //Exitoso!
        else
            perror("unspec:bind");
	}
	close(*sd);
	return -1;
}
