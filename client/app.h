#ifndef APP_H_
	#define APP_H_

	typedef struct {
		char server[16];
		int port;
	}clientConf;

	int establishConnection(int *sd, clientConf *conf);

#endif
