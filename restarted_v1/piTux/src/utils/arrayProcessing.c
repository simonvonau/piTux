#include "arrayProcessing.h"



int* initIntArray(int p_size,int p_defaultVal){
    int * res=malloc(p_size*sizeof(int));//faudra bien penser à faire un free !
    int i;
    for(i=0;i<p_size;i++){
        res[i]=p_defaultVal;
    }
    return res;
}//--------------------------------------------------------------------------------------------------------------------


char ** splitString(char* p_str,char p_separator,int p_sizeMax,int p_returnArrayMaxQty,int p_returnMaxSize){
// Split a string into an 2D array
    int i;
    char buff[p_returnMaxSize];
    int buffIndex;
    char **res = malloc(p_returnArrayMaxQty * sizeof(char *));
    int resIndex;

    //  Memory allocation
    for(i = 0; i < p_returnArrayMaxQty; i++){
        res[i]=malloc(p_returnMaxSize * sizeof(char));
    }
    // Splitting
    buffIndex = 0;
    resIndex = 0;
    for(i = 0;i < p_sizeMax && buffIndex < p_returnMaxSize && resIndex < p_returnArrayMaxQty; i++){
        if(p_str[i] == p_separator || i == p_sizeMax - 1 || buffIndex == p_returnMaxSize - 1){
            if(resIndex < p_returnArrayMaxQty){
                strncpy(res[resIndex], buff, p_returnMaxSize);
                res[resIndex][buffIndex] = '\0';
            }
            // Reinitialising  buffer
            memset(buff, 0, sizeof(buff));
            buffIndex = 0;
            resIndex += 1;
        }else{
            buff[buffIndex] = p_str[i];
            buffIndex += 1;
        }
    }
    return res;
}//--------------------------------------------------------------------------------------------------------------------


