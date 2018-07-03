#include "app.h"

#include <regex.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define BUFF_SIZE 1024  //El tamaño debe ir acorde al tamanio del archivo que va a leer
#define debug 1

int configServer(char *cfgfile, char *proto, char *port, char *mqName) {

	if (debug) printf("configServer - cfg: %s\n",cfgfile);

	char buff[BUFF_SIZE];
	int fd;
	
	memset (buff,'\0',sizeof buff);
	
	if ((fd = open(cfgfile, O_RDONLY)) < 0){
		perror ("open");
		return -1;
	}
	if (debug) printf("configServer - fd: %d\n",fd);
	
	if (read(fd,buff,sizeof buff) < 0){
		perror ("read");
		close(fd);
		return -1;
	}
	
	//Parse message queue name
	if (parse("logging_mq_name=.+(\\r|\\t|\\n|\\s)",buff,16,mqName) < 0){
		perror("parse");
		close(fd);
		return -1;
	}
	mqName[strnlen(mqName,128)-1]='\0';
	if (debug) printf("configServer - mqName: %s\n",mqName);

	//Parse port number
	if (parse("port=[0-9]?[0-9]?[0-9]?[0-9]?[0-9](\\r|\\t|\\n|\\s)",buff,5,port) < 0){
		perror("parse");
		close(fd);
		return -1;
	}
	port[strnlen(port,MAX_PORT_LENGTH)-1]='\0';
	if (debug) printf("configServer - port: %s\n",port);
	
	if ( strnlen(port,MAX_PORT_LENGTH) < MIN_PORT_LENGTH || strnlen(port,9) > MAX_PORT_LENGTH || atoi(port) == 0 ) { //Puerto invalido
		write(STDERR_FILENO,"Puerto invalido\n",16);
		close(fd);
		return -1;
	}

	//Parse protocoi
	if (parse("proto=[0-9](\\r|\\t|\\n|\\s)",buff,6,proto) < 0){
		perror("parse");
		close(fd);
		return -1;
	}
	proto[strnlen(proto,2)-1]='\0';
	if( atoi(proto) < 0 ) {
		write(STDERR_FILENO,"wrong protocol specified, the socket will be created using unspec family\n",73);
		proto="0";
	}
	if (debug) printf("configServer - protocol: %s\n",proto);

	close(fd);
    
	return 0;
}
