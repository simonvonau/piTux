#include "arrayProcessing.h"



int* initIntArray(int p_size,int p_defaultVal){
    int * res=malloc(p_size*sizeof(int));//faudra bien penser à faire un free !
    int i;
    for(i=0;i<p_size;i++){
        res[i]=p_defaultVal;
    }
    return res;
}//--------------------------------------------------------------------------------------------------------------------


void splitString(char* p_str, int p_strMaxSize, char p_separator, char ***p_res, int *p_resSize1, int *p_resSize2, int p_resMaxSize1, int p_resMaxSize2){
// Split a string into an 2D array
    int i;
    int rowCounter = 0;
    char buff[p_resMaxSize2];
    int buffIndex = 0;



    *p_res = malloc(p_resMaxSize1 * sizeof(char*));
    for(i = 0; i < p_resMaxSize1; i++){
        (*p_res)[i] = malloc(p_resMaxSize2 * sizeof(char));
    }

    // Splitting p_str
    for(i = 0; i < p_strMaxSize; i++){

        if((p_str[i] == p_separator || buffIndex >= p_resMaxSize2 || i == p_strMaxSize-1 || p_str[i] == '\0') && rowCounter < p_resMaxSize1){
            p_resSize2[rowCounter] = buffIndex;
            if(p_resSize2[rowCounter] > p_resMaxSize2){
                p_resSize2[rowCounter] = p_resMaxSize2;
            }

            memcpy((*p_res)[rowCounter], buff, p_resSize2[rowCounter] * sizeof(char));

            // Adding end of string character
            if(p_resSize2[rowCounter] >= p_resMaxSize2){
                (*p_res)[rowCounter][p_resMaxSize2 - 1] = '\0';
            }else{
                (*p_res)[rowCounter][p_resSize2[rowCounter]] = '\0';
                p_resSize2[rowCounter] += 1;
            }
            rowCounter += 1;
            buffIndex = 0;
        }else if( buffIndex < p_resMaxSize2){
            buff[buffIndex] = p_str[i];
            buffIndex += 1;
        }
    }
    *p_resSize1 = rowCounter;
}//--------------------------------------------------------------------------------------------------------------------



