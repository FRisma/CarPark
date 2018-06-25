#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define debug 0

const char * kResponseSimpleTemplate 		= "HTTP/1.1 %d %s\r\n\r\n";

const char * kResponseWithLocationTemplate 	= "HTTP/1.1 %d %s\r\n"\
											   "Content-Type: text/plain\r\n"\
											   "Content-Length: %lu\r\n"\
											   "Location: %s\r\n\r\n"\
											   "%s";

const char * kResponseWithBodyTemplate 		= "HTTP/1.1 %d %s\r\n"\
											  "Content-Type: text/plain\r\n"\
											  "Content-Length: %lu\r\n\r\n"\
											  "%s";

/* Headers */
int dispatch(int sd, char *payload);
int dispatchResponse(int sd, http_response resp);

/* Implementation */
int dispatchResponse(int sd, http_response resp) {

	if (debug) printf("dispatchResponse resp code: %d message %s body %s\n",resp.code,resp.statusM,resp.body);
	int d_code 		= floor(log10(abs(resp.code))) + 1; // http code number of digits
	size_t body_len = strnlen(resp.body,800); 			// response body length
	int d_body_len 	= floor(log10(abs(body_len))) + 1; // http body length number of digits

	char payload[1024] = {'\0'};
	switch(resp.code) {
		case kHttp200:
			if (debug) puts("Sending RespWithBody 200");
			snprintf(payload,\
					d_code + strlen(resp.statusM) + d_body_len + body_len + 58 + 1,\
					kResponseWithBodyTemplate,\
					resp.code,resp.statusM,body_len,resp.body);
			break;
		case kHttp201:
			if (debug) puts("Sending RespWithLocation 201");
			snprintf(payload,\
					d_code + strlen(resp.statusM) + d_body_len + 15 + body_len + 70 + 1,\
					kResponseWithLocationTemplate,\
					resp.code,resp.statusM,body_len,"alguna location",resp.body);
			break;
		case kHttp204:
		case kHttp400:
		case kHttp404:
		case kHttp500:
		case kHttp501:
			if (debug) puts("Sending RespSimple");
				snprintf(payload,\
					d_code + strlen(resp.statusM) + 1,\
					kResponseSimpleTemplate,\
					resp.code,resp.statusM);
			break;
		default:
			write(STDERR_FILENO,"Error: invalid http code\n",25);
			return -1;
	}
	
	if (-1 == dispatch(sd,payload)) {
		write(STDERR_FILENO,"Error: error dispatch\n",22);
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
