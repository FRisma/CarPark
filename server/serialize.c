#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

char * const kPattern = "#:%li|a:%d|c:%s|f:%d|o:%lu|i:%s|t:%s|b:%s";

#define debug 1

int serialize(slot s, char *stream) {

	int d_id 				= floor(log10(abs(s.id))) + 1;
	int d_available 		= floor(log10(abs(s.available))) + 1;
	int d_floor 			= floor(log10(abs(s.floor))) + 1;
	int d_offset 			= floor(log10(abs(s.offset))) + 1;
	char displayInTime[80]	= {'\0'};
	char displayOutTime[80]	= {'\0'};
	if (s.checkInTime) {
		//strftime(displayInTime,80,"%x - %I:%M%p", s.checkInTime);
		asctime_r(s.checkInTime,displayInTime);			// Thread safe
		displayInTime[strlen(displayInTime) -1] = '\0'; // Replace last char whis is \n with \0 to avoid line break
	} else {
		strncpy(displayInTime,"00:00\0",80);
	}
	if (s.checkOutTime) {
		asctime_r(s.checkInTime,displayOutTime);		  // Thread safe
		displayOutTime[strlen(displayOutTime) -1] = '\0'; // Replace last char whis is \n with \0 to avoid line break
	} else {
		strncpy(displayOutTime,"00:00\0",80);
	}
	if (debug) {
		puts("serialize node:");
		printLocation(&s);
	}
	if (0 > snprintf(stream,\
				d_id + d_available + strnlen(s.idCli,sizeof(s.idCli)) + d_floor + d_offset + strlen(displayInTime) + strlen(displayOutTime) + strlen(s.bill) + 24,\
				kPattern,\
				s.id, s.available, s.idCli, s.floor, s.offset, displayInTime, displayOutTime, s.bill) ) {
		write(STDERR_FILENO,"serialize snprintf",18);
		return -1;
	}

	if (debug) printf("Serialize Resultado %s\n",stream);
	return 0;
}
