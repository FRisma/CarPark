#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

char * const kPattern = "#:%li|a:%d|c:%s|f:%d|o:%lu|i:%s|t:%s|b:%s";

#define debug 0

int serialize(slot s, char *stream) {

	int d_id 				= floor(log10(abs(s.id))) + 1;
	int d_available 		= floor(log10(abs(s.available))) + 1;
	int d_floor 			= floor(log10(abs(s.floor))) + 1;
	int d_offset 			= floor(log10(abs(s.offset))) + 1;
	char displayInTime[80]	= {'\0'};
	char displayOutTime[80]	= {'\0'};
	
	if (NULL != s.checkInTime) {strftime(displayInTime, sizeof(displayInTime), "%c", s.checkInTime);}
	else { strncpy(displayInTime,"00:00",5);}
	if (NULL != s.checkOutTime) {strftime(displayOutTime, sizeof(displayOutTime), "%c", s.checkOutTime); }
	else {strncpy(displayOutTime,"00:00",5);}

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
