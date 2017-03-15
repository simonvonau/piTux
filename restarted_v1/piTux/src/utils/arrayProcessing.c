#include "arrayProcessing.h"



int* initIntArray(int pSize,int defaultVal){
    int * res=malloc(pSize*sizeof(int));//faudra bien penser à faire un free !
    int i;
    for(i=0;i<pSize;i++){
        res[i]=defaultVal;
    }
    return res;
}//--------------------------------------------------------------------------------------------------------------------


char ** splitString(char* str,char separator,int sizeMax,int returnArrayMaxQty,int returnMaxSize){
// Split a string into an 2D array
    int i;
    char buff[returnMaxSize];
    int buffIndex;
    char **res = malloc(returnArrayMaxQty * sizeof(char *));
    int resIndex;

    //  Memory allocation
    for(i = 0; i < returnArrayMaxQty; i++){
        res[i]=malloc(returnMaxSize * sizeof(char));
    }
    // Splitting
    buffIndex = 0;
    resIndex = 0;
    for(i = 0;i < sizeMax && buffIndex < returnMaxSize && resIndex < returnArrayMaxQty; i++){
        if(str[i] == separator || i == sizeMax - 1 || buffIndex == returnMaxSize - 1){
            if(resIndex < returnArrayMaxQty){
                strncpy(res[resIndex], buff, returnMaxSize);
                res[resIndex][buffIndex] = '\0';
            }
            // Reinitialising  buffer
            memset(buff, 0, sizeof(buff));
            buffIndex = 0;
            resIndex += 1;
        }else{
            buff[buffIndex] = str[i];
            buffIndex += 1;
        }
    }
    return res;
}//--------------------------------------------------------------------------------------------------------------------


