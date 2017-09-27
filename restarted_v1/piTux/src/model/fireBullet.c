#include "fireBullet.h"


FireBullet * initFireBullet( char *p_path){
// Initialize a p_fireBullet
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    FireBullet *res = malloc(sizeof(FireBullet));

    if(res == NULL){
        reportErreur("Error malloc initFireBullet()");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                res->refColl = initNonRegisteredCollider(atoi(buff[3]),atoi(buff[4]), 0, 0, 1, atoi(buff[2]), state_normal);
                res->speedX = atoi(buff[5]);
                res->speedY = atoi(buff[6]);
                res->maxLifeTime = atoi(buff[7]);
                res->maxJumpHeight = atoi(buff[8]);
            }else if(strcmp(buff[0], "[Action]") == 0){
                res->spritesSize1 = atoi(buff[2]);
                res->spriteDuration = atoi(buff[3]);
                res->sprites = malloc( res->spritesSize1 * sizeof(SDL_Surface *));
                if(res->sprites == NULL){
                    reportErreur("Error malloc initFireBullet():1");
                }
            }else if (strcmp(buff[0], "[Sprite]") == 0){
                res->sprites[atoi(buff[1])] = loadImage(buff[2]);
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

void destroyFireBullet(FireBullet *p_fireBullet){
// Free a fireBullet
    int i,j;
    for( i = 0; i < p_fireBullet->spritesSize1; i++){
        SDL_FreeSurface(p_fireBullet->sprites[i]);
    }
    free(p_fireBullet->sprites);
    destroyCollider(p_fireBullet->refColl);
    free(p_fireBullet);
}//------------------------------------------------------------------------------------------------------------------------
