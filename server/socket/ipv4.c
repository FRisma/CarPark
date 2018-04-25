#include "mySocket.h"

#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>

int ipv4(int *sd,char *puerto){

	struct sockaddr_in dir = {};
	memset(&dir,0,sizeof(struct sockaddr_in));
	
	int opc = 1;

	if ( (*sd = socket(PF_INET,SOCK_STREAM,0)) < 0 ){
		perror("socket ipv4");
		return -1;
	}
	dir.sin_family = AF_INET;
	dir.sin_port = htons(atoi(puerto));
	dir.sin_addr.s_addr = INADDR_ANY;
    if (bind(*sd,(struct sockaddr *)&dir,sizeof dir) < 0){
		perror ("bind_ipv4");
		return -1;
	}
	if ( setsockopt(*sd, SOL_SOCKET, SO_REUSEADDR, (char *) &opc, sizeof(opc)) <0 ){
		perror("setsockopt()");
		return -1;
	}
	return 0;
}
