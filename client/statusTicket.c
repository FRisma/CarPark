#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char *kStatusURI = "slot/%s";

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

	return 0;
}
