#include "processRequest.h"
#include "utils/utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define debug 1

int processRequest(int sd, http_request *req) {

	int leido;
	char buffer[1024] = {'\0'};;
	char tmp[255] = {'\0'};

	//memset
		
	while( (leido = read(sd,buffer,sizeof buffer)) > 0 ) {
		if (-1 == leido) {
			perror("processRequest - read");
			close(sd);
			return -1;
		}

		if (debug) write(STDOUT_FILENO,buffer,leido);
		if (debug) write(STDOUT_FILENO,"\n",1);

		/* ---------------- Parsing ----------------------- */
		char *outer_ptr=NULL;
		
		/* Method */
		char *aux = (char*)malloc(sizeof(buffer));
		strncpy(aux,buffer,strnlen(buffer,sizeof(buffer)));
		char *tmp2 = strtok_r(aux, " ", &outer_ptr);
		if ( 0 == strncmp("POST",tmp2,strnlen(tmp2,4)) )
			req->method = POST;
		if ( 0 == strncmp("GET",tmp2,strnlen(tmp2,3)) )
			req->method = GET;
		if ( 0 == strncmp("PUT",tmp2,strnlen(tmp2,3)) )
			req->method = PUT;
		if ( 0 == strncmp("DELETE",tmp2,strnlen(tmp2,6)) )
			req->method = DELETE;
		if ( 0 == strncmp("HEAD",tmp2,strnlen(tmp2,4)) )
			req->method = HEAD;
		if ( 0 == strncmp("OPTIONS",tmp2,strnlen(tmp2,7)) )
			req->method = OPTIONS;
		if (debug) printf("processRequest - method: %d\n",req->method);

		/* Resource */
		memset(tmp,'\0',sizeof(tmp));
		if ( parse("POST /.*([\r\n])",buffer,6,tmp) < 0){
			perror("parse method");
			close(sd);
			return -1;
		}
		tmp[strlen(tmp)-1]='\0';
		if (debug) printf("processRequest - resource: %s\n",tmp);

		/* Content Type */
		memset(tmp,'\0',sizeof(tmp));
		if ( parse("Content-Type: .*([\r\n])",buffer,14,tmp) < 0){
			perror("parse method");
			close(sd);
			return -1;
		}
		tmp[strlen(tmp)-1]='\0';
		if (debug) printf("processRequest - CT: %s\n",tmp);

		/* Content Length */
		memset(tmp,'\0',sizeof(tmp));
		if ( parse("Content-Length: .*([\r\n])",buffer,16,tmp) < 0){
			perror("parse method");
			close(sd);
			return -1;
		}
		tmp[strlen(tmp)-1]='\0';
		if (debug) printf("processRequest - CL: %s\n",tmp);


		/* Body */



		/*if ( 0 > write(sd,buffer,leido) ) {
			perror("Thread - write");
			free(responseHeader);
			close(sd);
			pthread_exit(NULL);
		}*/
	}

	return 0;
}
