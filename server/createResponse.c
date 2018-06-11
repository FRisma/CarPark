#include "createResponse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int createResponse(http_request *req, http_response **resp) {

	if (!req) {
		return -1;		
	}

	switch(req->methods) {
		case GET:
			// Find slot
			break;
		case POST:
			// Create
			break;
		case PUT:
			// Update
			break;
		case DELETE:
			// Remove
			break;
		default:
			// Metodo no implementado aun o desconocido
	}
}

