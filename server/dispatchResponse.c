#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 1

const char * kResponseOKTemplate = "HTTP/1.1 %d %s\r\n\r\n";

const char * kResponseOKWithLocationTemplate = "HTTP/1.1 %d %s\r\n"\
											   "Content-Type: text/plain\r\n"\
											   "Content-Length: %lu\r\n"\
											   "Location: %s\r\n\r\n"\
											   "%s";

const char * kResponseOKWithBodyTemplate 	= "HTTP/1.1 %d %s\r\n"\
											  "Content-Type: text/plain\r\n"\
											  "Content-Length: %lu\r\n\r\n"\
											  "%s";

const char * kResponseErrorWithBodyTemplate = "HTTP/1.1 %d %s\r\n";

const char * kResponseErrorTemplate			= "HTTP/1.1 %d %s\r\n\r\n"\
									 		  "Content-Type: text/plain\r\n"\
									   		  "Content-Length: %lu\r\n\r\n"\
									 		  "%s";

/* Headers */
int dispatch(int sd, char *payload);
int dispatchResponse(int sd, http_response *resp);

/* Implementation */
int dispatchResponse(int sd, http_response *resp) {

	if (debug) printf("dispatchResponse resp code: %d message %s body %s\n",resp->code,resp->statusM,resp->body);

	char payload[1024] = {'\0'};

	puts("dispatchResponse antes del snprintf");
	//snprintf(payload, 100, kResponseOkWithLocationTemplate,1,"A","","");
	snprintf(payload, strlen(kResponseOKWithLocationTemplate) + 3 + 2 + 10 + 70 + 1,kResponseOKWithLocationTemplate,201,"Created",50,"slot/10001",resp->body);

	puts("dispatchResponse antes del switch");
	switch(resp->code) {
		case kHttp200:
			if (debug) puts("Sending 200 response");
			//snprintf(payload,  ,kResponseOKTemplate,arg);
			break;
		case kHttp201:
			if (debug) puts("Sending 201 response");
			//snprintf(payload,len,kResponseOKTemplate,arg);
		case kHttp204:
			if (debug) puts("Sending 204 response");
			//snprintf(payload,len,kResponseOKTemplate,arg);
		case kHttp400:
			if (debug) puts("Sending 400 response");
			//snprintf(payload,len,kResponseErrorTemplate,arg);
			break;
		case kHttp404:
			if (debug) puts("Sending 404 response");
			//snprintf(payload,len,kResponseErrorTemplate,arg);
			break;
		case kHttp500:
			if (debug) puts("Sending 500 response");
			//snprintf(payload,len,kResponseErrorTemplate,arg);
			break;
		case kHttp501:
			if (debug) puts("Sending 501 response");
			//snprintf(payload,len,kResponseErrorTemplate,arg);
			break;
		default:
			write(STDERR_FILENO,"invalid http code\n",18);
			return -1;
	}
	
	if (-1 == dispatch(sd,payload)) {
		write(STDERR_FILENO,"error dispatch\n",15);
		return -1;
	}

	return 0;
}

int dispatch(int sd, char *payload) {

	if (debug) printf("Writing to sd: %d this: %s\n",sd,payload);
	if (-1 == write(sd,payload,strlen(payload))) {
		perror("write");
		return -1;
	}
	return 0;
}
