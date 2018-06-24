#include "response.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define debug 1

const char * kMessageFor200 = "OK";
const char * kMessageFor201 = "Created";
const char * kMessageFor204 = "No Content";
const char * kMessageFor400 = "Bad Request";
const char * kMessageFor404 = "Not Found";
const char * kMessageFor500 = "Internal Server Error";
const char * kMessageFor501 = "Not Implemented";

int response(int httpCode, char *body, http_response **resp) {

	//Check if resp exist, if not, the process will crash
	if (**response == NULL) {
		write(STDERR_FILENO,"response resp has no memory allocated\n",38);
		return -1;
	}
	switch(httpCode) {
		case kHttp200:
			if (debug) puts("response.c 200");
			strncpy((*resp)->statusM,kMessageFor200,strlen(kMessageFor200));
			break;
		case kHttp201:
			if (debug) puts("response.c 201");
			strncpy((*resp)->statusM,kMessageFor201,strlen(kMessageFor201));
			break;
		case kHttp204:
			if (debug) puts("response.c 204");
			strncpy((*resp)->statusM,kMessageFor204,strlen(kMessageFor204));
			break;
		case kHttp400:
			if (debug) puts("response.c 400");
			strncpy((*resp)->statusM,kMessageFor400,strlen(kMessageFor400));
			break;
		case kHttp404:
			if (debug) puts("response.c 404");
			strncpy((*resp)->statusM,kMessageFor404,strlen(kMessageFor404));
			break;
		case kHttp500:
			if (debug) puts("response.c 500");
			strncpy((*resp)->statusM,kMessageFor500,strlen(kMessageFor500));
			break;
		case kHttp501:
			if (debug) puts("response.c 501");
			strncpy((*resp)->statusM,kMessageFor501,strlen(kMessageFor501));
			break;
		default:
			write(STDERR_FILENO,"response.c http code not recognized\n",36);
			return -1;
	}

	(*resp)->code = httpCode;
	printf("Resp code %d status %s ",(*resp)->code,(*resp)->statusM);
	if ( 0 < strlen(body) ) {
		strncpy( (*resp)->body,body,strlen(body) );
		printf("Body %s\n",(*resp)->body);
	}

	
	return 0;
}
