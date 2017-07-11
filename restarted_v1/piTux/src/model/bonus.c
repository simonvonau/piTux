#include "bonus.h"

Bonus ** initBonusArray(char *path, int *resSize){
// Initialize a Bonus array
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    Bonus **res = NULL;
    int resCurrIndex = 0;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                *resSize = atoi(buff[1]);
                res = malloc(*resSize * sizeof(Bonus *));
                if(res == NULL){
                    *resSize = 0;
                    reportErreur("Error malloc initBonus2()");
                }
            }else if(strcmp(buff[0], "[Bonus]") == 0 && resCurrIndex < *resSize){
                res[resCurrIndex] = initBonus(buff[2]);
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

Bonus * initBonus(char *path){
// Initialize a Bonus
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    Bonus *res = malloc(sizeof(Bonus));
    int currentAction = 0;

    if(res == NULL){
        reportErreur("Error malloc initBonus()");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                res->spritesSize1 = atoi(buff[1]);
                res->bonusType = atoi(buff[2]);
                res->sprites = malloc( res->spritesSize1 * sizeof(SDL_Surface **));
                res->spritesSize2 = malloc( res->spritesSize1 * sizeof(int));
                res->spriteDuration = malloc( res->spritesSize1 * sizeof(int));
                res->refColl = initNonRegisteredCollider(atoi(buff[3]),atoi(buff[4]), 0, 0, 1, 0);
                res->speedX = atoi(buff[5]);
                res->speedY = atoi(buff[6]);

                if(res->sprites == NULL || res->spritesSize2 == NULL || res->spriteDuration == NULL){
                    reportErreur("Error malloc initBonus():1");
                }
            }else if(strcmp(buff[0], "[Action]") == 0 && currentAction < res->spritesSize1){
                currentAction = atoi(buff[1]);
                res->spritesSize2[currentAction] = atoi(buff[2]);
                res->spriteDuration[currentAction] = atoi(buff[3]);
                res->sprites[currentAction] = malloc(res->spriteDuration[currentAction] * sizeof(SDL_Surface *));
                if(res->sprites[currentAction] == NULL ){
                    reportErreur("Error malloc initBonus():2");
                }
            }else if (strcmp(buff[0], "[Sprite]") == 0){
                res->sprites[currentAction][atoi(buff[1])] = loadImage(buff[2]);
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

void destroyBonus(Bonus *currBonus){
// Free a bonus
    int i,j;
    for( i = 0; i < currBonus->spritesSize1; i++){
        for(j = 0; j < currBonus->spritesSize2[i]; j++){
            SDL_FreeSurface(currBonus->sprites[i][j]);
        }
        free(currBonus->sprites[i]);
    }
    free(currBonus->sprites);

    free(currBonus->spriteDuration);
    free(currBonus->spritesSize2);
    destroyCollider(currBonus->refColl);
    free(currBonus);
}//------------------------------------------------------------------------------------------------------------------------
