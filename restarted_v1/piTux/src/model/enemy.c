#include "enemy.h"

Enemy **initEnemyArray(char *path,int *resSize){
// Load an array of enemies
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    Enemy **res;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                *resSize = atoi(buff[1]);
                res = malloc(*resSize * sizeof(Enemy *));
                if(res == NULL){
                    reportErreur("Error malloc initEnemyArray():1");
                }
            }else if(strcmp(buff[0], "[Enemy]") == 0){
                res[atoi(buff[1])] = initEnemy(buff[2]);
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
Enemy *initEnemy(char *path){
// Load an enemy from file
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    int i;
    Enemy *res = malloc(sizeof(Enemy));
    int currentAction = 0;

    if(res == NULL){
        reportErreur("Error malloc initEnemy():1");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                res->spritesSize1 = atoi(buff[1]);
                res->sprites = malloc( res->spritesSize1 * sizeof(SDL_Surface **));
                res->spritesSize2 = malloc( res->spritesSize1 * sizeof(int));
                res->spriteDuration = malloc( res->spritesSize1 * sizeof(int));
                res->speed = malloc(res->spritesSize1 * sizeof(int));
                res->actionColl = malloc(res->spritesSize1 * (sizeof(Collider *)));
                res->enemyType = atoi(buff[2]);


                if(res->sprites == NULL || res->spritesSize2 == NULL || res->spriteDuration == NULL || res->speed == NULL
                   || res->actionColl == NULL){
                    reportErreur("Error malloc initEnemy():2");
                }
            }else if(strcmp(buff[0], "[Action]") == 0 && currentAction < res->spritesSize1){
                currentAction = atoi(buff[1]);
                res->spritesSize2[currentAction] = atoi(buff[2]);
                res->spriteDuration[currentAction] = atoi(buff[3]);
                res->sprites[currentAction] = malloc(res->spriteDuration[currentAction] * sizeof(SDL_Surface *));
                res->actionColl[currentAction] = initNonRegisteredCollider(atoi(buff[4]), atoi(buff[5]), 0, 0, 1, 0);
                res->speed[currentAction] = atoi(buff[6]);

                if(res->sprites[currentAction] == NULL ){
                    reportErreur("Error malloc initEnemy():3");
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
void destroyEnemy(Enemy *currEnemy){
// Free enemy memory
    int i,j;
    for(i = 0; i < currEnemy->spritesSize1; i++){
        for(j = 0; j < currEnemy->spritesSize2[i]; j++){
            SDL_FreeSurface(currEnemy->sprites[i][j]);
        }
        free(currEnemy->sprites[i]);
        destroyCollider(currEnemy->actionColl[i]);
    }
    free(currEnemy->sprites);
    free(currEnemy->actionColl);

    free(currEnemy->speed);
    free(currEnemy->spriteDuration);
    free(currEnemy->spritesSize2);
    free(currEnemy->specialBehaviour);
    free(currEnemy);
}//------------------------------------------------------------------------------------------------------------------------

