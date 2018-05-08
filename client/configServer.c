#include "app.h"
#include "utils/utils.h"

#include <regex.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFF_SIZE 1024  //El tamaño debe ir acorde al tamanio del archivo que va a leer
#define debug 1

int configServer(char *cfgfile, char *ip, char *port) {

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

	if (parse("ip=[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}(\\r|\\t|\\n|\\s)",buff,3,ip) < 0){
		perror("parse ip");
		close(fd);
		return -1;
	} 
	if (debug) printf("configServer - ip: %s\n",ip);
	ip[strlen(ip)-1]='\0';

	/*if (parse("port=[0-9]?[0-9]?[0-9]?[0-9]?[0-9](\\r|\\t|\\n|\\s)",buff,5,port) < 0){
		perror("parse port");
		close(fd);
		return -1;
	} 
	port[strlen(port)-1]='\0';
	*/
	strncpy(port,"2587",5);
	if (debug) printf("configServer - port: %s\n",port);
	
	if (strlen(port) < 1) { //Puerto invalido
		write(STDERR_FILENO,"Puerto invalido\n",16);
		close(fd);
		return -1;
	}
	close(fd);
    
	return 0;
}
