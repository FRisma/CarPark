#ifndef MYSOCKET_H_
	#define MYSOCKET_H_

	int protocol_handler(int protoId,int *sd,char *port);
	int unspec(int* sd,const char *port);
	int ipv4(int * sd, char *port);
	int ipv6(int * sd, char *port);

#endif
