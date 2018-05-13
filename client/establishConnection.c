/*
 * It will configure the socket and then it will establish a connection
 *
 */

#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

int establishConnection(int *sd, clientConf *conf){

    struct sockaddr_in serverSock = {0}; //Inicializa la estructura con 0

    //Protocolo
    serverSock.sin_addr.s_addr=inet_addr(conf->ip);
    serverSock.sin_family=AF_INET;
    serverSock.sin_port=htons(atoi(conf->port));
    if ( 0 > (*sd=socket(AF_INET,SOCK_STREAM,0)) ) { perror("socket"); return -1; }
    
    if ( 0 != connect(*sd,(struct sockaddr *)&serverSock,sizeof serverSock) ){  //CONNECT Con el servidor
            perror ("connect");
            return -1;
    }

    return 0;
}
