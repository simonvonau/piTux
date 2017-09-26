#include "translation.h"

Translation ** initTranslations(char *p_path, int *p_resSize, int *p_nbLanguages, Translation **p_languagesList){
// Init an array of Translation*
    FILE *file = fopen ( p_path, "r" );
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
                *p_nbLanguages = atoi(buff[1]);
                *p_resSize = atoi(buff[2]);
                // All langages list
                *p_languagesList = initTranslation(*p_nbLanguages, lineSizeMax, 3, buff);
                res = malloc(*p_resSize * sizeof(Translation *));
                if(res == NULL){
                    *p_resSize = 0;
                    reportErreur("Error malloc initTranslations()");
                }
            }else if(strcmp(buff[0], "[Data]") == 0 && resCurrIndex < *p_resSize){
                res[resCurrIndex] = initTranslation(*p_nbLanguages, lineSizeMax, 2, buff);
                resCurrIndex += 1;
            }
        }

        fclose ( file );
    }else{
        perror ( p_path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Translation* initTranslation(int p_nbLanguages, int p_sentenceSize, int p_startIndex, char ** p_sentence){
    int i;
    Translation *res =malloc(sizeof(Translation));
    if( res == NULL){reportErreur("initTranslation(...):error malloc(...)");}
    res->nbLanguages = p_nbLanguages;
    res->sentenceSize = p_sentenceSize;
    res->sentence = malloc(p_nbLanguages * sizeof(char *));

    for(i = 0; i < p_nbLanguages; i++){
        res->sentence[i] = malloc(p_sentenceSize * sizeof(char));
        if (res->sentence[i] == NULL){
            reportErreur("Error malloc initTranslation()");
        }else{
            strncpy(res->sentence[i], p_sentence[p_startIndex+i], p_sentenceSize);
        }
    }
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyTranslation(Translation *p_translation){
    int i;

    for(i = 0; i < p_translation->nbLanguages; i++){
        free(p_translation->sentence[i]);
    }

    free(p_translation->sentence);
    free(p_translation);
}//------------------------------------------------------------------------------------------------------------------------
