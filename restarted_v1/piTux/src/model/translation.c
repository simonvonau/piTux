#include "translation.h"

Translation ** initTranslations(char *path, int *resSize, int *nbLangages, Translation **langagesList){
// Init an array of Translation*
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 15;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff = malloc(nbMaxElemPerLine * sizeof(char *));
    char line[lineSizeMax];
    int i;
    Translation **res = NULL;
    int resCurrIndex = 0;


    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                *nbLangages = atoi(buff[1]);
                *resSize = atoi(buff[2]);
                // All langages list
                *langagesList = initTranslation(*nbLangages, lineSizeMax, 3, buff);
                res = malloc(*resSize * sizeof(Translation *));
                if(res == NULL){
                    *resSize = 0;
                    reportErreur("Error malloc initTranslations()");
                }
            }else if(strcmp(buff[0], "[Data]") == 0 && resCurrIndex < *resSize){
                res[resCurrIndex] = initTranslation(*nbLangages, lineSizeMax, 2, buff);
                resCurrIndex += 1;
            }
        }

        fclose ( file );
    }else{
        perror ( path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Translation* initTranslation(int currNbLangages, int currSentenceSize, int startIndex, char ** currSentence){
    int i;
    Translation *res =malloc(sizeof(Translation));
    if( res == NULL){reportErreur("initTranslation(...):error malloc(...)");}
    res->nbLangages = currNbLangages;
    res->sentenceSize = currSentenceSize;
    res->sentence = malloc(currNbLangages * sizeof(char *));

    for(i = 0; i < currNbLangages; i++){
        res->sentence[i] = malloc(currSentenceSize * sizeof(char));
        if (res->sentence[i] == NULL){
            reportErreur("Error malloc initTranslation()");
        }else{
            strncpy(res->sentence[i], currSentence[startIndex+i], currSentenceSize);
        }
    }
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyTranslation(Translation *currTranslation){
    int i;

    for(i = 0; i < currTranslation->nbLangages; i++){
        free(currTranslation->sentence[i]);
    }

    free(currTranslation->sentence);
    free(currTranslation);
}//------------------------------------------------------------------------------------------------------------------------
