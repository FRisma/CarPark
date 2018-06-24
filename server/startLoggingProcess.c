#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug 0

int startLoggingProcess(char *arg) {

	//int status;
	pid_t parent_pid;
	pid_t child_pid;

	parent_pid = getpid();

	child_pid = fork();
	switch(child_pid) {
		case -1:
			puts("Logging procces cant be started");
			perror("fork");
			break;
		case 0: // Son
			execlp("../logger/logger",arg,NULL);
			// should not excecute following lines if everything was ok, so next line, should be processed only if there
			// was an error
			perror("exec");
			exit(EXIT_FAILURE);
			break;
		default: // Dad
			//no errors
			break;
	}

	if (debug) {
		printf( "[startLoggingProcess] parent PID: %d\n", parent_pid );
		printf( "[startLoggingProcess] child PID: %d\n", child_pid );
	}

	return 0;
}
