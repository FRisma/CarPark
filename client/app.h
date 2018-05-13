#ifndef APP_H_
	#define APP_H_

	#include "utils/utils.h"

	#define MAX_IP_LENGTH 16
	#define MIN_IP_LENGTH 7
	#define MAX_PORT_LENGTH 6
	#define MIN_PORT_LENGTH 2

	typedef struct {
		char ip[MAX_IP_LENGTH];
		char port[MAX_PORT_LENGTH];
	}clientConf;

	int establishConnection(int *sd, clientConf *conf);

	int configServer(char *cfgfile, char *ip, char *port);

	void createTicket(int socketDescriptor);
	void deleteTicket(int socketDescriptor, char *slotId);
	void statusTicket(int socketDescriptor, char *slotId);

#endif
