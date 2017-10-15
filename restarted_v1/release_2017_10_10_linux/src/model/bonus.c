#include "bonus.h"

Bonus ** initBonusArray(char *p_path, int *p_resSize){
// Initialize a Bonus array
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Bonus **res = NULL;
    int resCurrIndex = 0;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 2 && strcmp(buff[0], "[Header]") == 0){
                *p_resSize = atoi(buff[1]);
                res = malloc(*p_resSize * sizeof(Bonus *));
                if(res == NULL){
                    *p_resSize = 0;
                    reportErreur("Error malloc initBonus2()");
                }
            }else if(buffSize1 >= 3 && strcmp(buff[0], "[Bonus]") == 0 && resCurrIndex < *p_resSize){
                res[resCurrIndex] = initBonus(buff[2]);
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

}//------------------------------------------------------------------------------------------------------------------------

Bonus * initBonus(char *p_path){
// Initialize a Bonus
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Bonus *res = malloc(sizeof(Bonus));
    int currentAction = 0;

    if(res == NULL){
        reportErreur("Error malloc initBonus()");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 7 && strcmp(buff[0], "[Header]") == 0){
                res->spritesSize1 = atoi(buff[1]);
                res->bonusType = atoi(buff[2]);
                res->sprites = malloc( res->spritesSize1 * sizeof(SDL_Surface *));
                res->spritesSize2 = malloc( res->spritesSize1 * sizeof(int));
                res->spriteDuration = malloc( res->spritesSize1 * sizeof(int));
                res->refColl = initNonRegisteredCollider(atoi(buff[3]),atoi(buff[4]), 0, 0, 1, res->bonusType, state_normal);
                res->speedX = atoi(buff[5]);
                res->speedY = atoi(buff[6]);

                if(res->sprites == NULL || res->spritesSize2 == NULL || res->spriteDuration == NULL){
                    reportErreur("Error malloc initBonus():1");
                }
            }else if(buffSize1 >= 4 && strcmp(buff[0], "[Action]") == 0 && currentAction < res->spritesSize1){
                currentAction = atoi(buff[1]);
                res->spritesSize2[currentAction] = atoi(buff[2]);
                res->spriteDuration[currentAction] = atoi(buff[3]);
                res->sprites[currentAction] = malloc(res->spriteDuration[currentAction] * sizeof(SDL_Surface *));
                if(res->sprites[currentAction] == NULL ){
                    reportErreur("Error malloc initBonus():2");
                }
            }else if (buffSize1 >= 3 && strcmp(buff[0], "[Sprite]") == 0){
                res->sprites[currentAction][atoi(buff[1])] = loadImage(buff[2]);
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
}//------------------------------------------------------------------------------------------------------------------------

void destroyBonus(Bonus *p_bonus){
// Free a bonus
    int i,j;
    for( i = 0; i < p_bonus->spritesSize1; i++){
        for(j = 0; j < p_bonus->spritesSize2[i]; j++){
            SDL_FreeSurface(p_bonus->sprites[i][j]);
        }
        free(p_bonus->sprites[i]);
    }
    free(p_bonus->sprites);

    free(p_bonus->spriteDuration);
    free(p_bonus->spritesSize2);
    destroyCollider(p_bonus->refColl);
    free(p_bonus);
}//------------------------------------------------------------------------------------------------------------------------
