#include "myUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int Malloc(char **variable, long size){

    if ( NULL == ( *variable = (char *) malloc (size) )) { perror("malloc"); return -1;}
    
    memset (*variable, '\0', size);
    return 0;
}

