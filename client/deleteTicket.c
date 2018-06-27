#include "app.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define debug 0

const char *kDeleteURI = "/slots/%s";

int deleteTicket(int socketDescriptor, char *slotId) {

	char httpRequest[256] = {'\0'};
	char httpResponse[512] = {'\0'};
	char resource[16] = {'\0'};

	snprintf(resource, strlen(kDeleteURI) + strnlen(slotId,SLOT_ID_LENGTH) + 1, kDeleteURI, slotId);
	snprintf(httpRequest, strlen(HTTP_DELETE) + strlen(resource) + 1, HTTP_DELETE, resource); 

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
		printf("No se encuentra una posicion ocupada actualmente con para el id %s\n",slotId);
	} else {
		puts("Se dio de baja");
		printLocation(&result);
	}
	
	return 0;
}
