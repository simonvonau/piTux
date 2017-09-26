#include "heros.h"




Heros *initHeros(char p_path[FILE_PATH_SIZE_MAX]){
// Init a new Heros
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    Heros *res = malloc(sizeof(Heros));
    int currentTuxState = 0;
    int currentTuxAction = 0;
    if(res == NULL){
        reportErreur("initHeros(): error malloc()");
    }

    if(file != NULL ){
        while(fgets(line, sizeof(line), file) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                res->id = atoi(buff[1]);
                res->stateSize = atoi(buff[2]);
                res->speed = atoi(buff[3]);
                res->jumpSpeed = atoi(buff[4]);
                res->jumpDuration = atoi(buff[5]);

                res->sprites = malloc(res->stateSize * sizeof(SDL_Surface ***));
                res->spriteDuration = malloc(res->stateSize * sizeof(int *));
                res->herosColl = malloc(res->stateSize * sizeof(Collider **));
                res->actionSize = malloc(res->stateSize * sizeof(int));
                res->spriteSize = malloc(res->stateSize * sizeof(int *));

                if(res->sprites == NULL || res->herosColl == NULL || res->actionSize == NULL || res->spriteSize == NULL || res->spriteDuration == NULL){
                    reportErreur("initHeros(): error malloc() 2");
                }
            }else if(strcmp(buff[0], "[State]") == 0){
                res->actionSize[atoi(buff[1])] = atoi(buff[2]);

                res->sprites[atoi(buff[1])] = malloc(atoi(buff[2]) * sizeof(SDL_Surface **));
                res->spriteDuration[atoi(buff[1])] = malloc(atoi(buff[2]) * sizeof(int));
                res->herosColl[atoi(buff[1])] = malloc(atoi(buff[2]) * sizeof(Collider *));
                res->spriteSize[atoi(buff[1])] = malloc(atoi(buff[2]) * sizeof(int));
                if(res->sprites[atoi(buff[1])] == NULL || res->herosColl[atoi(buff[1])] == NULL
                   || res->spriteSize[atoi(buff[1])] == NULL || res->spriteDuration[atoi(buff[1])] == NULL){
                    reportErreur("initHeros(): error malloc() 3");
                }
                currentTuxState += 1;
                currentTuxAction = 0;
            }else if(strcmp(buff[0], "[Action]") == 0){
                res->spriteDuration[currentTuxState - 1][atoi(buff[1])] = atoi(buff[3]);
                res->spriteSize[currentTuxState - 1][atoi(buff[1])] = atoi(buff[2]);
                res->herosColl[currentTuxState - 1][atoi(buff[1])] = initNonRegisteredCollider(atoi(buff[4]), atoi(buff[5]), 0, 0, 0, TAG_HEROS_TUX);

                res->sprites[currentTuxState - 1][atoi(buff[1])] = malloc(atoi(buff[2]) * sizeof(SDL_Surface *));
                if (res->sprites [currentTuxState - 1][atoi(buff[1])] == NULL){
                    reportErreur("initHeros(): error malloc() 4");
                }
                currentTuxAction += 1;
            }else if (strcmp(buff[0], "[Sprite]") == 0){
                res->sprites [currentTuxState - 1][currentTuxAction - 1][atoi(buff[1])] = loadImage(buff[2]);
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



    /*res->currState = 0;
    res->currAction = 0;
    res->currSprite = 0;
    res->isDead = 0;
    res->lifesLeft = 2;
    res->nbCoins = 0;
    res->lastMovementTime = 0;
    res->timeBeforeIdle = 2000;
    res->isTouchingGround = 0;
    res->jumpKeyPressed = 0;
    res->jumpStartTime = 0;
    res->posX = 100;
    res->posY = 400;
    res->lastX = 0;
    res->lastY = 0;
    res->currentTime = 0;*/
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyHeros(Heros *p_heros){
// Free Heros memory
    int i, j, k;
    for(i = 0; i < p_heros->stateSize; i++){
        for(j = 0; j < p_heros->actionSize[i]; j++){
            for(k = 0; k < p_heros->spriteSize[i][j]; k++){
                SDL_FreeSurface(p_heros->sprites[i][j][k]);
            }
            free(p_heros->sprites[i][j]);
            destroyCollider(p_heros->herosColl[i][j]);
        }
        free(p_heros->sprites[i]);
        free(p_heros->herosColl[i]);
        free(p_heros->spriteDuration[i]);
        free(p_heros->spriteSize[i]);
    }
    free(p_heros->sprites);
    free(p_heros->spriteDuration);
    free(p_heros->herosColl);
    free(p_heros->actionSize);
    free(p_heros->spriteSize);

    free(p_heros);
}//------------------------------------------------------------------------------------------------------------------------

