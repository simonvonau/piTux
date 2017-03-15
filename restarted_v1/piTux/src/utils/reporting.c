#include "reporting.h"


void reportErreur(char * description){
    printf("ERROR: %s \n",description);
    exit(EXIT_FAILURE);
}//------------------------------------------------------------------------------------------------------------------------
