#ifndef RESPONSE_H_
	#define RESPONSE_H_

	#define kHttp200 200
	#define kHttp201 201 //should return a location
	#define kHttp204 204
	#define kHttp400 400
	#define kHttp404 404
	#define kHttp500 500
	#define kHttp501 501

	typedef struct response{
		int code;
		char statusM[64];
		char body[1024];
	}http_response;

	int response(int code, char *body, http_response *resp);

#endif
