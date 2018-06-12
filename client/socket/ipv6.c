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

int ipv6(int *sd,char *puerto){
	
	struct sockaddr_in6 dir = {0};
	int opc = 1;


	*sd = socket(AF_INET6,SOCK_STREAM,0);
	if (*sd < 0){
		perror("socket ipv6");
		return -1;
	}
	dir.sin6_family = AF_INET6;
	dir.sin6_port = htons(atoi(puerto));
	dir.sin6_addr = in6addr_any;
	if (bind(*sd,(struct sockaddr *)&dir,sizeof dir) < 0){
		perror("bind ipv6");
		return -1;
	}
	if ( setsockopt(*sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opc, sizeof(opc)) <0 ){
		perror("setsockopt()");
		return -1;
	}

	return 0;
}
