#ifndef APP_H_
	#define APP_H_

	typedef struct {
		char serverIp[16];
		char port[6];
	}clientConf;

	int establishConnection(int *sd, clientConf *conf);

	int configServer(char *cfgfile, char *ip, char *port);

#endif
