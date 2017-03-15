#include "levelIcon.h"


LevelIcon **initLevelIcons(char *path, int *resSize){
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff = malloc(nbMaxElemPerLine * sizeof(char *));
    char line[lineSizeMax];
    int i;
    LevelIcon **res = NULL;
    int resCurrIndex=0;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                *resSize = atoi(buff[1]);
                res = malloc(*resSize * sizeof(LevelIcon *));
                if(res == NULL){
                    *resSize = 0;
                    reportErreur("Error malloc initLevelIcon()");
                }
            }else if(strcmp(buff[0], "[Data]") == 0 && resCurrIndex < *resSize){
                res[resCurrIndex] = initLevelIcon(lineSizeMax, 1, buff);
                resCurrIndex += 1;
            }
        }

        fclose ( file );
    }else{
        perror ( path );
    }

    for(i=0;i < nbMaxElemPerLine; i++){
        free(buff[i]);
    }
    free(buff);

    return res;

}//--------------------------------------------------------------------------------------------------------------------

LevelIcon *initLevelIcon(int lineSizeMax, int startIndex, char ** buff){
    LevelIcon *res = malloc(sizeof(LevelIcon));
    strncpy(res->path, buff[startIndex], lineSizeMax);
    res->posX = atoi(buff[startIndex + 1]);
    res->posY = atoi(buff[startIndex + 2]);
    res->isCleared = atoi(buff[startIndex + 3]);
    res->isAvailable = atoi(buff[startIndex + 4]);
    res->linkedLevel1 = atoi(buff[startIndex + 5]);
    res->linkedLevel2 = atoi(buff[startIndex + 6]);
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyLevelIcon(LevelIcon *currLevelIcon){
    free(currLevelIcon);
}//--------------------------------------------------------------------------------------------------------------------
