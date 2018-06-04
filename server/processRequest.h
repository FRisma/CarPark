#ifndef PROCESSREQUEST_H_
	#define PROCESSREQUEST_H_

	//include http constants
	typedef enum methods { 
		GET, 
		POST, 
		PUT, 
		UPDATE, 
		DELETE, 
		HEAD, 
		OPTIONS
	} http_methods;

	typedef struct request {
		http_methods method;
		char resource[255];
		char content_type[255];
		unsigned long content_lenght;
		char *body;
	} http_request;

	/* Will read from the socket and parse the info to fill in the htttp_request struct */
	int processRequest(int socket, http_request *req);

#endif
