#ifndef __bErrors_h__
#define __bErrors_h__

#include <stdlib.h>
#include <stdio.h>

#define bErrorNull(pointer, errMSG, ...)\
        do {if(pointer == NULL){fprintf(stderr,errMSG,##__VA_ARGS__);exit(EXIT_FAILURE);}}while (0)

#endif