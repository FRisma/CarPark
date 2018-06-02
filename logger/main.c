#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#define MAX_SIZE 1024
#define MSG_STOP "exit"
#define debug 0

int main (int argc, char *const argv[]) {
	
	puts("\t\t ..:: Trabajo Final - Computacion II - CarPark Activity Logging ::..");

	if (2 != argc) {
		perror("no enough args");
		exit(EXIT_FAILURE);
	}
	char mq_name[255];
	char tmp[255];
	
	/* mq_overview() set to 255 the max of the mq name */
	strncpy(tmp, argv[1], strnlen(argv[1],255));
	snprintf(mq_name,strlen(tmp)+2,"/%s",tmp);

	mqd_t mq;
	struct mq_attr attr;
	char buffer[MAX_SIZE + 1];
	int must_stop = 0;

	/* initialize the queue attributes */
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_SIZE;
	attr.mq_curmsgs = 0;

	/* create the message queue */
	if (debug) printf("Queue name: %s\n", mq_name);
	if ( -1 == (mq= mq_open(mq_name, O_CREAT | O_RDONLY, 0644, &attr)) ) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}

	do {
		ssize_t bytes_read;

		/* receive the message */
		if (-1 == (bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL)) ) {
			perror("mq_receive");
			exit(EXIT_FAILURE);
		}

		buffer[bytes_read] = '\0';
		if (! strncmp(buffer, MSG_STOP, strlen(MSG_STOP))) {
			must_stop = 1;
		 } else {
			 printf("Received: %s\n", buffer);
			 // Do something, save the message
		 }
	} while (!must_stop);

	/* cleanup */
	if (-1 ==mq_close(mq)) {
		perror("mq_close");
	}
	if (-1 != mq_unlink(mq_name)) {
		perror("mq_unkink");
	}

	return 0;
}
