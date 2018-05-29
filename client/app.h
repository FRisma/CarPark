#ifndef APP_H_
	#define APP_H_

	#include "utils/utils.h"
	#include "constants.h"

	typedef struct {
		char ip[MAX_IP_LENGTH];
		char port[MAX_PORT_LENGTH];
	}clientConf;

	typedef struct pos {
		long int id;
		long int idCli;
		int floor;
		unsigned int offset;
		unsigned long checkInTime;
	}slot;

	int establishConnection(int *sd, clientConf *conf);

	int configServer(char *cfgfile, char *ip, char *port);

	void createTicket(int socketDescriptor);
	void deleteTicket(int socketDescriptor, char *slotId);
	void statusTicket(int socketDescriptor, char *slotId);

#endif
