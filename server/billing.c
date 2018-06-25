#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug 0

const char *kCurrencyFormat = "$";
const char *kBillPattern = "%s%lu";

int billing(slot *current) {

	if (NULL != current && current->id != -1) {
		char aPagar[16] = {'\0'};
		// calcular las horas a partir de checkInTime hasta checkOutTime
		// multiplicar las horas obtenidas por el precio por
		// hora o fraccion y guardar el resultado nuevamente en
		// el slot
		snprintf(aPagar, strlen(kCurrencyFormat) + 2 + 1,kBillPattern,kCurrencyFormat,15);
		strncpy(current->bill,aPagar,strlen(aPagar));
		if (debug) printf("El monto a pagar es %s\n",current->bill);
	} else {
		write(STDERR_FILENO,"No slot for billing\n",20);
		return -1;
	}

	return 0;
}
