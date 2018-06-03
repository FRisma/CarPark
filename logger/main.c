#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>

#define MSG_MAX_SIZE 1024
#define MSG_MAX_POOL 10
#define MSG_KEY_STOP "__STOP__LOGGING__"
#define debug 1

int main (int argc, char *const argv[]) {
	
	puts("\t\t ..:: Trabajo Final - Computacion II - CarPark Activity Logging ::..");

	if (2 != argc) {
		write(STDERR_FILENO,"No enough args\n",15);
		exit(EXIT_FAILURE);
	}
	char mq_name[255] = {'\0'};
	char tmp[255] = {'\0'};
	
	/* mq_overview() set to 255 the max of the mq name */
	strncpy(tmp, argv[1], strnlen(argv[1],255));
	snprintf(mq_name,strlen(tmp)+2,"/%s",tmp);

	mqd_t mq;
	struct mq_attr attr;
	char buffer [MSG_MAX_SIZE + 1];
	int must_stop = 0;

	/* initialize the queue attributes */
	attr.mq_flags = 0; /* Set this flag to O_NONBLOCK to make the sender fails immediately if the queue is full to prevent blocking it */
	attr.mq_maxmsg = MSG_MAX_POOL;
	attr.mq_msgsize = MSG_MAX_SIZE; /* in bytes */
	attr.mq_curmsgs = 0;

	/* create the message queue */
	if (debug) printf("Queue name: %s\n", mq_name);
	if ( -1 == (mq= mq_open(mq_name, O_CREAT | O_RDONLY, 0644, &attr)) ) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}

	/* Open-Create the file in the filesysistem */
	int fd = 0; 
	if ( -1 == (fd = open("records.log", O_WRONLY | O_CREAT | O_APPEND,S_IRWXU)) ) {
		perror("open");
		return -1;
	}


	ssize_t bytes_read;
	do {
		/* receive the message */
		if (-1 == (bytes_read = mq_receive(mq, buffer, MSG_MAX_SIZE, NULL)) ) {
			perror("mq_receive");
			exit(EXIT_FAILURE);
		}

		buffer[bytes_read] = '\0';
		if (! strncmp(buffer, MSG_KEY_STOP, strnlen(MSG_KEY_STOP,50))) {
			must_stop = 1;
		 } else {
			 printf("Received: %s\n", buffer);
			 // Write the message to the file
			 write(fd,buffer,bytes_read);
			 write(fd,"\n",1);
		 }
	} while (!must_stop);

	/* cleanup */
	if (-1 == close(fd)) {
		perror("close");
	}
	if (-1 ==mq_close(mq)) {
		perror("mq_close");
	}
	if (-1 != mq_unlink(mq_name)) {
		perror("mq_unkink");
	}

	return 0;
}
