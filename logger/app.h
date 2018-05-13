#ifndef APP_H_
#define APP_H_

	#define QUEUE_NAME  "/carpark_log_queue"
	#define MAX_SIZE    1024
	#define MSG_STOP    "exit"

	#define CHECK(x) \
	    do { \
			if (!(x)) { \
				fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
				perror(#x); \
				exit(-1); \
			} \
		} while (0) \

#endif
