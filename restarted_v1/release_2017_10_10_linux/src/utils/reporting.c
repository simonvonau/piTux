#include "reporting.h"


void reportErreur(char * p_description){
    printf("ERROR: %s \n", p_description);
    exit(EXIT_FAILURE);
}//------------------------------------------------------------------------------------------------------------------------
