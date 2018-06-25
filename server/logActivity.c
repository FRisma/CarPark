#include "app.h"
#include "requestHeader.h"

#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define debug 1

const char * kMessageTemplate = "door:%s|method:%s|resource:%s|time:%s";

int logActivity(mqd_t messageQueue, http_request req, int mode) {
	
	if (0 < messageQueue) {
		/* Format req to message */
		char message[128] = {'\0'};
		char displayMethod[8] = {'\0'};

		switch(req.method) {
			case GET:
				strcpy(displayMethod,"GET");
				break;
			case POST:
				strcpy(displayMethod,"POST");
				break;
			case PUT:
				strcpy(displayMethod,"PUT");
				break;
			case DELETE:
				strcpy(displayMethod,"DELETE");
				break;
			case HEAD:
				strcpy(displayMethod,"HEAD");
				break;
			case OPTIONS:
				strcpy(displayMethod,"OPTIONS");
				break;
			case UPDATE:
				strcpy(displayMethod,"UPDATE");
				break;
		}
		if (debug) printf("logActivity method %s\n",displayMethod);
		if (debug) printf("logActivity resource %s\n",req.resource);
		
		snprintf(message,\
				7 + strlen(displayMethod) + strlen(req.resource) + 5 + 29 + 1,\
				kMessageTemplate,\
				"puertax",displayMethod,req.resource,"00:00");

		if (debug) printf("logActivity mode %d - Message %s\n",mode,message);

		/* using mq_timesend because if the queue is full we don't want to block the thread for a long period of time */
		struct timespec waitTime = {0};
		waitTime.tv_sec = 1; 	/* seconds */
		waitTime.tv_nsec = 0; 	/* nano seconds */

		switch(mode) {
			case 0:
			default:
				if ( -1 == mq_timedsend(messageQueue, message, strlen(message), 0, &waitTime) ) {
					perror("mq_timesend");
					return -1;
				}
		}
	} else {
		write(STDERR_FILENO,"E: invalid message queue descriptor\n",37);
		return -1;
	}

	return 0;
}
