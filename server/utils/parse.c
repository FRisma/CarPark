#include "parse.h"

#include <regex.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

const char *parseError = "parse error\n";

#define debug 0

int parse (char *patron, char *buff, int offset, char *resultado){

	if (debug) printf("parse.c entrada: %s", buff);

	regex_t retmp;
	regmatch_t mtmp;

	if (buff == NULL){
		write(STDERR_FILENO,"buffer vacio",12);
		return -1;
	}

	if ( (regcomp(&retmp,patron,REG_EXTENDED) != 0)){
		perror("regcomp()");
		return -1;
	}

	if ( !(regexec(&retmp, buff ,(size_t) 1 , &mtmp, 0)) ){	
		memset(resultado,'\0',strnlen(resultado,2*1024));
		strncpy(resultado,buff+(mtmp.rm_so+offset),mtmp.rm_eo-(mtmp.rm_so+offset));
		if (debug) printf("parse.c resultado: %s\n",resultado);
		return 1; //Devuelve la cadena con un \n al final
	}

	if (debug) write(STDERR_FILENO,"parse.c Nothing found\n",22);
	return 0;
}
