#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char *kStatusURI = "/slots/%s";

#define debug 0

int statusTicket(int socketDescriptor, char *slotId) {

	char httpRequest[256] = {'\0'};
	char httpResponse[512] = {'\0'};
	char resource[16] = {'\0'};

	snprintf(resource, strlen(kStatusURI) + strnlen(slotId,SLOT_ID_LENGTH) + 1, kStatusURI, slotId);
	snprintf(httpRequest, strlen(HTTP_GET) + strlen(resource) + 1, HTTP_GET, resource); 

	if (-1 == dispatch(socketDescriptor, httpRequest, httpResponse) ) {
		return -1;
	}

	printf("El estado actual de la posicion %s es\n%s\n",slotId,httpResponse);
	
	return 0;
}
