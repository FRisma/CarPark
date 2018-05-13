#include "app.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//static char * const e403_code="HTTP/1.0 403 FORBIDDEN\r\nContent-Type: text/html\n\n<html>\n<body>\n403 Access Denied\n</body>\n</html>";
static char * const e403_code="403 FORBIDDEN";
static char * const e404_code="404 NOT FOUND";
static char * const e500_code="500 Server Internal Error";

static char * const e403_message="<html>\n<body>\n403 You don't have permission to view this file\n</body>\n</html>";
static char * const e404_message="<html>\n<body>\n404 File not found\n</body>\n</html>";
static char * const e500_message="<html>\n<body>\n500 Something went terribly wrong\n</body>\n</html>";

static char * const errorTemplate="HTTP/1.0 %s\r\nContent-Type: text/html\n\n%s";

#define debug 1

void* threadWork(void *data) {
	
	// Set this thread as detached
	pthread_detach(pthread_self());
	
	threadData *arg = (threadData *)data;
		
	// Map structure into local variables
	int newSocketDescriptor = arg->csd;
	
	// Map message queue
	printf("MQ arg: %d",arg->mqd);
	mqd_t mq = arg->mqd;
	printf("MQ loc: %d",mq);

	char *responseHeader = NULL;
	if ( (responseHeader = (char *)malloc(1024)) == NULL ) {
		perror("malloc responseHeader");
	}


	int leido=0;
	char buffer[1024];
		        
	if (debug) printf("ThreadArgs->connectionSD:%d threadArgs->mqd:%d\n",newSocketDescriptor, mq);
	while( (leido = read(newSocketDescriptor,buffer,sizeof buffer)) > 0 ) {
		if (debug) write(STDOUT_FILENO,buffer,leido);
		write(newSocketDescriptor,buffer,leido);
		/*							
		switch ( request(buffer,arg->droot,newSocketDescriptor) ){
			case 200: // Termina exitosamente
				if (debug) puts("200OK File served");
				break;
			case 404: // Archivo no encontrado
				if (debug) puts("File not found");
				if ( snprintf(responseHeader,strlen(e404_code)+strlen(e404_message)+37,errorTemplate,e404_code,e404_message) < 0 ) {
					perror("snprintf error404");
				}
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 404");
				}
				break;
			case 403: // Forbidden
				if (debug) puts("File permissions issue");
				if ( snprintf(responseHeader,strlen(e403_code)+strlen(e403_message)+37,errorTemplate,e403_code,e403_message) < 0 ) {
					perror("snprintf erro403");
				}		
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 403");
				}
				break;
			case 500: // Problemas del server
			default:
				if (debug) puts("Something went wrong");
				if ( snprintf(responseHeader,strlen(e500_code)+strlen(e500_message)+37,errorTemplate,e500_code,e500_message) < 0 ) {
					perror("snprintf error500");
				}
				if ( write(newSocketDescriptor,responseHeader,strlen(responseHeader)) < 0){
					perror("write 500");
				}
				break;
		}
											
		//if ( close(newSocketDescriptor) == -1 ) {
		//	perror("close");
		//}
		//pthread_exit(NULL);
		*/
		mq_send(mq, "Hola desde el hiloserver", 21, 0);
	}

	return 0;
}
