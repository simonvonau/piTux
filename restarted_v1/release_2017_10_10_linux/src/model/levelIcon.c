#include "levelIcon.h"


LevelIcon **initLevelIcons(char *p_path, int *p_resSize){
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    LevelIcon **res = NULL;
    int resCurrIndex=0;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 2 && strcmp(buff[0], "[Header]") == 0){
                *p_resSize = atoi(buff[1]);
                res = malloc(*p_resSize * sizeof(LevelIcon *));
                if(res == NULL){
                    *p_resSize = 0;
                    reportErreur("Error malloc initLevelIcon()");
                }
            }else if(buffSize1 >= 1 && strcmp(buff[0], "[Data]") == 0 && resCurrIndex < *p_resSize){
                res[resCurrIndex] = initLevelIcon(LINE_SIZE_MAX, 1, buff);
                resCurrIndex += 1;
            }
        }

        fclose ( file );
    }else{
        perror ( p_path );
    }
    for(i = 0; i < buffSize1; i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//--------------------------------------------------------------------------------------------------------------------

LevelIcon *initLevelIcon(int p_lineSizeMax, int p_startIndex, char ** p_buff){
// Init a levelIcon from the data in the p_buff param
    LevelIcon *res = malloc(sizeof(LevelIcon));
    strncpy(res->path, p_buff[p_startIndex], p_lineSizeMax);
    res->posX = atoi(p_buff[p_startIndex + 1]);
    res->posY = atoi(p_buff[p_startIndex + 2]);
    res->isCleared = atoi(p_buff[p_startIndex + 3]);
    res->isAvailable = atoi(p_buff[p_startIndex + 4]);
    res->linkedLevel1 = atoi(p_buff[p_startIndex + 5]);
    res->linkedLevel2 = atoi(p_buff[p_startIndex + 6]);
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyLevelIcon(LevelIcon *p_levelIcon){
    free(p_levelIcon);
}//--------------------------------------------------------------------------------------------------------------------
