#include "block.h"

Block ** initBlockArray(char *p_path, int *p_resSize){
    // Initialize a block array
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the fileLINE_SIZE_MAX
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Block **res = NULL;
    int resCurrIndex = 0;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 2 && strcmp(buff[0], "[Header]") == 0){
                *p_resSize = atoi(buff[1]);
                res = malloc(*p_resSize * sizeof(Block *));
                if(res == NULL){
                    *p_resSize = 0;
                    reportErreur("Error malloc initBlocks()");
                }
            }else if( buffSize1 >= 3 && strcmp(buff[0], "[Block]") == 0 && resCurrIndex < *p_resSize){
                res[resCurrIndex] = initBlock(buff[2]);
                resCurrIndex += 1;
            }
        }

        fclose (file);
    }else{
        perror (p_path);
    }
    for(i = 0; i < buffSize1; i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Block * initBlock(char *p_path){
    // Initialize 1 block
    int i;
    FILE *file = fopen (p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different value in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Block *res = malloc(sizeof(Block));
    int currentAction = 0;

    if(res == NULL){
        reportErreur("Error malloc initBlock()");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            // Loading the header
            if(buffSize1 >= 6 && strcmp(buff[0], "[Header]") == 0){
                res->spritesSize1 = atoi(buff[1]);
                res->blockType = atoi(buff[2]);
                res->sprites = malloc( res->spritesSize1 * sizeof(SDL_Surface *));
                res->spritesSize2 = malloc( res->spritesSize1 * sizeof(int));
                res->spriteDuration = malloc( res->spritesSize1 * sizeof(int));
                res->refColl = initNonRegisteredCollider(atoi(buff[3]), atoi(buff[4]), 0, 0, 1, res->blockType, state_normal);
                res->speedY = atoi(buff[5]);

                if(res->sprites == NULL || res->spritesSize2 == NULL || res->spriteDuration == NULL){
                    reportErreur("Error malloc initBlock():1");
                }

            // Loading a specific action
            }else if(buffSize1 >= 4 && strcmp(buff[0], "[Action]") == 0 && currentAction < res->spritesSize1){
                currentAction = atoi(buff[1]);
                res->spritesSize2[currentAction] = atoi(buff[2]);
                res->spriteDuration[currentAction] = atoi(buff[3]);
                res->sprites[currentAction] = malloc(res->spriteDuration[currentAction] * sizeof(SDL_Surface *));
                if(res->sprites[currentAction] == NULL ){
                    reportErreur("Error malloc initBlock():2");
                }

            // loading all the sprites for an action
            }else if (buffSize1 >= 2 && strcmp(buff[0], "[Sprite]") == 0){
                res->sprites[currentAction][atoi(buff[1])] = loadImage(buff[2]);
            }
        }
        fclose (file);
    }else{
        perror (p_path);
    }
    for(i = 0; i < buffSize1; i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyBlock(Block *p_block){
// Free block memory
    int i,j;
    for( i = 0; i < p_block->spritesSize1; i++){
        for(j = 0; j < p_block->spritesSize2[i]; j++){
            SDL_FreeSurface(p_block->sprites[i][j]);
        }
        free(p_block->sprites[i]);
    }
    free(p_block->sprites);

    free(p_block->spriteDuration);
    free(p_block->spritesSize2);
    destroyCollider(p_block->refColl);
    free(p_block);
}//------------------------------------------------------------------------------------------------------------------------
