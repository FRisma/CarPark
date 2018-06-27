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
		write(STDERR_FILENO,"dispatch\n",9);
		return -1;
	}

	slot result = {'\0'};
	result.id = -1;
	memset(result.idCli,'\0',10);
	memset(result.checkInTime,'\0',200);
	memset(result.checkOutTime,'\0',200);
	memset(result.bill,'\0',16);
	if (-1 == deserialize(httpResponse, &result)) {
		write(STDERR_FILENO,"deserialize\n",12);
		return -1;
	}
	
	if (0 > result.id) {
		printf("No se ha encontrado un nodo con el id %s\n",slotId);
	} else {
		puts("El estado del nodo es");
		printLocation(&result);
	}
	
	return 0;
}
