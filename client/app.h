#ifndef APP_H_
	#define APP_H_

	#include "constants.h"
	#include "utils/utils.h"
	#include <stdlib.h>

	typedef enum {
		false,
		true
	}bool;

	typedef struct {
		char ip[MAX_IP_LENGTH];
		char port[MAX_PORT_LENGTH];
	}clientConf;

	typedef struct slot{
		long int id;
		bool available;
		char idCli [10];
		int floor;
		size_t offset;
		struct tm *checkInTime;
		struct tm *checkOutTime;
		char bill [16];
		struct slot *next;
	}slot;

	int establishConnection(int *sd, clientConf *conf);

	int configServer(char *cfgfile, char *ip, char *port);

	int createTicket(int socketDescriptor);
	int deleteTicket(int socketDescriptor, char *slotId);
	int statusTicket(int socketDescriptor, char *slotId);

	/* Convert the slot structre into a serialized string ready to be sent */
	int serialize(slot s, char *serialized);
	
	/* Send thru a sd descriptor the stream of bytes stored in req, the answer will be stored in resp */
	int dispatch(int sd, char *req, char *resp);

#endif
