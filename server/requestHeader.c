#include "requestHeader.h"
#include "utils/utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define debug 1

const char *kct			=	"Content-Type";
const char *kcl			=	"Content-Length";
const char *kget		=	"GET";
const char *kpost		=	"POST";
const char *kput		=	"PUT";
const char *kdelete		=	"DELETE";

// Prototype
int parseMethodAndResource(char *line, http_methods *method, char *resource);
int parseHeaderLine (char *line, http_request *req);

// Implementation
int requestHeader(int sd, http_request *req) {

	int leido;
	char buffer[1024] = {'\0'};;
	char header[1024] = {'\0'};;
	char aux[1024] = {'\0'};

	char *tmp, *ptr = NULL;

	while( (leido = read(sd,buffer,sizeof(buffer))) > 0 ) {
		if (-1 == leido) {
			perror("processRequest - read");
			return -1;
		}
		if (debug) {write(STDOUT_FILENO,buffer,leido); write(STDOUT_FILENO,"\n",1);}

		/* ---------------- START HEADER PARSE ----------------------- */

		if ( parse(".*\r\n\r\n",buffer,0,aux) < 0){
			write(STDERR_FILENO,parseError,strlen(parseError));
			return -1;
		}
		
		if (strnlen(aux,sizeof(aux)) != 0) { // Si el header existe, entonces lo parseo
			strncpy(header,aux,strnlen(aux,sizeof(header)));
			if (debug) printf("--------- La cabecera es:\n%s--------------------\n",header);

			tmp = strtok_r(aux,"\n",&ptr);
			parseMethodAndResource(tmp, &(req->method), req->resource);

			if ( POST == req->method || PUT == req->method ) {
				/* Si es POST o PUT tiene content type, content length en el header */
				while ( (tmp = strtok_r(NULL,"\r",&ptr)) != NULL ) {
					/* Parse each header line */
					parseHeaderLine(tmp,req);
				}
			}
		} else {
			write(STDERR_FILENO,"No HTTP request's header found\n",33);
			return -1;
		}
		
		//sino sigo leyendo y lo appendeo a lo que ya leyo
		// ---------------------------- END HEADER PARSE -----------------------------------	
		
		/* Body */
		if ( (req->method == POST || req->method == PUT) && req->content_length > 0 ) {
			memset(aux,'\0',sizeof(aux));
			if ( parse("\r\n\r\n.*",buffer,4,aux) < 0){
				write(STDERR_FILENO,parseError,strlen(parseError));
				return -1;
			}
			if ( NULL == (req->body = (char *)malloc(req->content_length + 1)) ) {
				perror("malloc");
				return -1;
			}
			memset(req->body,'\0',(req->content_length + 1));
			strncpy(req->body,aux,req->content_length);
			if (debug) printf("processRequest - BODY: %s\n",req->body);
		}

		// Si la longitud del metdo, del recurso, y, si el metodo es post, el content length y el body son mayores que
		// 0, entonces romper el bucle.
		break;
	}

	return 0;
}

int parseMethodAndResource(char *line, http_methods *method, char *resource) {
	/* Method */
	char *ptr = NULL;
	char *tmp = strtok_r(line," ",&ptr);
	if ( 0 == strncasecmp(kpost,tmp,strnlen(kpost,4)) ) 			{ *method = POST; }
	else if ( 0 == strncasecmp(kget,tmp,strnlen(kget,3)) ) 			{ *method = GET; }
	else if ( 0 == strncasecmp(kput,tmp,strnlen(kput,3)) ) 			{ *method = PUT; }
	else if ( 0 == strncasecmp(kdelete,tmp,strnlen(kdelete,6)) ) 	{ *method = DELETE; }
	else 															{ write(STDERR_FILENO,"no HTTP method detected\n",24); return -1; }

	/* Resource */
	tmp = strtok_r(NULL," ",&ptr);
	strncpy(resource,tmp,strlen(tmp));
	
	return 0;
}

int parseHeaderLine (char *line, http_request *req) {

	// TODO mover a una funcion este filtrado
	char filtered[256] = {'\0'};
	unsigned int i,j = 0;
	for (i = 0; i<strlen(line); i++) {
		if ( line[i] == '\r' || line[i] == '\n' || line[i] == ' ' ) {
			continue;
		}
		filtered[j] = line[i];
		j++;
	}

	char *key, *value, *ptr = NULL;
	key = strtok_r(filtered,":",&ptr);
	value = strtok_r(NULL,": \r\n",&ptr);
		
	if ( key != NULL && value != NULL ) {
		if ( 0 == strcasecmp(kct,key) ) {
			strncpy(req->content_type,value,255);
			if (debug) printf("processRequest - CT: %s\n",req->content_type);
		}
		else if ( 0 == strcasecmp(kcl,key) ) {
			req->content_length = atol(value);
			if (debug) printf("processRequest - CL: %li\n",req->content_length);
		}
		else if ( 0 == strcasecmp("Host",key) ) {
			printf("Host: %s\n",value);
		}
		else if ( 0 == strcasecmp("Accept",key) ) {
			printf("Accept: %s\n",value);
		}
	}
	return 0;
}
