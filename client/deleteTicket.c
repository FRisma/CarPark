#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define debug 0

const char *kDeleteURI = "slot/%s";

int deleteTicket(int socketDescriptor, char *slotId) {

	char httpRequest[256] = {'\0'};
	char httpResponse[512] = {'\0'};
	char resource[16] = {'\0'};

	snprintf(resource, strlen(kDeleteURI) + strnlen(slotId,SLOT_ID_LENGTH) + 1, kDeleteURI, slotId);
	snprintf(httpRequest, strlen(HTTP_DELETE) + strlen(resource) + 1, HTTP_DELETE, resource); 

	if (-1 == dispatch(socketDescriptor, httpRequest, httpResponse) ) {
		return -1;
	}

	if (debug) printf("Dar de baja la posicion %s resultado:\n%s\n",slotId,httpResponse);
	
	return 0;
}
