/*
 * It will configure the socket and then it will establish a connection
 *
 */

#include "client.h"
#include "macros.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

int establishConnection(int *sd, file_conf_client *conf){

    struct sockaddr_in serverSock = {0}; //Inicializa la estructura con 0

    //Protocolo
    serverSock.sin_addr.s_addr=inet_addr(conf->serverIp);
    serverSock.sin_family=AF_INET;
    serverSock.sin_port=htons(conf->port);
    if ( 0 > (*sd=socket(AF_INET,SOCK_STREAM,0)) ) { perror("socket"); return -1; }
    
    if ( 0 != connect(*sd,(struct sockaddr *)&serverSock,sizeof serverSock) ){  //CONNECT Con el servidor
            perror ("connect");
            return -1;
    }

    return 0;
}
