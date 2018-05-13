#ifndef APP_H_
	#define APP_H_

	#include "utils/utils.h"
	
	typedef struct {
		char serverIp[16];
		char port[6];
	}clientConf;

	int establishConnection(int *sd, clientConf *conf);

	int configServer(char *cfgfile, char *ip, char *port);

	void createTicket(int socketDescriptor);
	void deleteTicket(int socketDescriptor, char *slotId);
	void statusTicket(int socketDescriptor, char *slotId);

#endif
