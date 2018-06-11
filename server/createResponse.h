#ifndef CREATERESPONSE_H_
	#define CREATERESPONSE_H_

	#include "processRequest.h"

	/* Check HTTP's RFC 2616 */
	const char *HTTPVER = "HTTP/1.1"
	const char *HTTP200 = "%s 200 OK\r\n";
	const char *HTTP201 = "%s 201 CREATED\r\n";
	const char *HTTP400 = "%s 400 BAD REQUEST\r\n";
	const char *HTTP401 = "%s 401 UNAUTHORIZED\r\n";
	const char *HTTP403 = "%s 403 FORBIDDEN\r\n";
	const char *HTTP404 = "%s 404 NOT FOUND\r\n";
	const char *HTTP500 = "%s 500 INTERNAL SERVER ERROR\r\n";

	typedef struct response {
		char statusLine[128];
		char *headers;
		char *body;
	}http_response;

	int createResponse(http_request *req, http_response **resp);

#endif
