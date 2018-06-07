#include "processRequest.h"
#include "utils/utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *kregexPattern	=	"%s .*\r\n";
const char *kct				=	"Content-Type:";
const char *kcl				=	"Content-Length:";
const char *kget			=	"GET";
const char *kpost			=	"POST";
const char *kput			=	"PUT";
const char *kdelete			=	"DELETE";

#define debug 0

int processRequest(int sd, http_request *req) {

	int leido;
	char buffer[1024] = {'\0'};;
	char aux[1024] = {'\0'};
	char tmp[255] = {'\0'};
	char expression[128] = {'\0'};

	char *tmp2;	
	char *outer_ptr=NULL;
	
	while( (leido = read(sd,buffer,sizeof(buffer))) > 0 ) {
		if (-1 == leido) {
			perror("processRequest - read");
			return -1;
		}
		if (debug) write(STDOUT_FILENO,buffer,leido);
		if (debug) write(STDOUT_FILENO,"\n",1);

		/* ---------------- Parsing ----------------------- */

		/* Method */
		strncpy(aux,buffer,strnlen(buffer,sizeof(aux)));
		tmp2 = strtok_r(aux, " ", &outer_ptr);
		if ( 0 == strncmp(kpost,tmp2,strnlen(kpost,4)) )
			req->method = POST;
		if ( 0 == strncmp(kget,tmp2,strnlen(kget,3)) )
			req->method = GET;
		if ( 0 == strncmp(kput,tmp2,strnlen(kput,3)) )
			req->method = PUT;
		if ( 0 == strncmp(kdelete,tmp2,strnlen(kdelete,6)) )
			req->method = DELETE;
		if ( 0 == strncmp("HEAD",tmp2,strnlen(tmp2,4)) )
			req->method = HEAD;
		if ( 0 == strncmp("OPTIONS",tmp2,strnlen(tmp2,7)) )
			req->method = OPTIONS;
		
		if ( 0 >= req->method ) {
			write(STDERR_FILENO,"no HTTP method detected\n",24);
			return -1;
		}
		if (debug) printf("processRequest - method: %d\n",req->method);

		/* Resource */
		memset(tmp,'\0',sizeof(tmp));
		snprintf(expression,strlen(tmp2)+strlen(kregexPattern)+1,kregexPattern,tmp2);
		if ( parse(expression,buffer,strnlen(tmp2,6),tmp) < 0) {
			write(STDERR_FILENO,parseError,strlen(parseError));
			return -1;
		}
		tmp2= strtok_r(tmp, " ", &outer_ptr);
		strncpy(req->resource,tmp2,strlen(tmp2));
		if (debug) printf("processRequest - resource: %s\n",req->resource);

		/* Content Type */
		memset(tmp,'\0',sizeof(tmp));
		snprintf(expression,strlen(kct)+strlen(kregexPattern)+1,kregexPattern,kct);
		if ( parse(expression,buffer,14,tmp) < 0 ) {
			write(STDERR_FILENO,parseError,strlen(parseError));
			return -1;
		}
		tmp[strlen(tmp)-1]='\0';
		strncpy(req->content_type,tmp,255);
		if (debug) printf("processRequest - CT: %s\n",req->content_type);

		/* Content Length */
		memset(tmp,'\0',sizeof(tmp));
		snprintf(expression,strlen(kcl)+strlen(kregexPattern)+1,kregexPattern,kcl);
		if ( parse(expression,buffer,16,tmp) < 0){
			write(STDERR_FILENO,parseError,strlen(parseError));
			return -1;
		}
		tmp[strlen(tmp)-1]='\0';
		req->content_length = atol(tmp);
		if (debug) printf("processRequest - CL: %li\n",req->content_length);

		/* Body */
		if (0 < req->content_length) {
			memset(aux,'\0',sizeof(aux));
			if ( parse("\r\n\r\n.*}",buffer,4,aux) < 0){
				write(STDERR_FILENO,parseError,strlen(parseError));
				return -1;
			}
			aux[strlen(aux)-1]='\0';
			req->body = (char *)malloc(req->content_length);
			strncpy(req->body,aux,req->content_length);
			if (debug) printf("processRequest - BODY: %s\n",req->body);
		}

		// Si la longitud del metdo, del recurso, y, si el metodo es post, el content length y el body son mayores que
		// 0, entonces romper el bucle.
		break;
	}

	return 0;
}
