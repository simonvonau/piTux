#include "enemy.h"

Enemy **initEnemyArray(char *p_path,int *p_resSize){
// Load an array of enemies
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Enemy **res;

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 2 && strcmp(buff[0], "[Header]") == 0){
                *p_resSize = atoi(buff[1]);
                res = malloc(*p_resSize * sizeof(Enemy *));
                if(res == NULL){
                    reportErreur("Error malloc initEnemyArray():1");
                }
            }else if(buffSize1 >= 3 && strcmp(buff[0], "[Enemy]") == 0){
                res[atoi(buff[1])] = initEnemy(buff[2]);
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

Enemy *initEnemy(char *p_path){
// Load an enemy from file
    int i;
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10; // Max different values in one line of the file
    char  **buff;
    int buffSize1;
    int buffSize2[nbMaxElemPerLine];
    char line[LINE_SIZE_MAX];
    Enemy *res = malloc(sizeof(Enemy));
    int currentAction = 0;

    if(res == NULL){
        reportErreur("Error malloc initEnemy():1");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            splitString(line, LINE_SIZE_MAX, ';', &buff, &buffSize1, buffSize2, nbMaxElemPerLine, LINE_SIZE_MAX);

            if(buffSize1 >= 3 && strcmp(buff[0], "[Header]") == 0){
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
            }else if(buffSize1 >= 7 && strcmp(buff[0], "[Action]") == 0 && currentAction < res->spritesSize1){
                currentAction = atoi(buff[1]);
                res->spritesSize2[currentAction] = atoi(buff[2]);
                res->spriteDuration[currentAction] = atoi(buff[3]);
                res->sprites[currentAction] = malloc(res->spriteDuration[currentAction] * sizeof(SDL_Surface *));
                res->actionColl[currentAction] = initNonRegisteredCollider(atoi(buff[4]), atoi(buff[5]), 0, 0, 1, res->enemyType, atoi(buff[7]));
                res->speed[currentAction] = atoi(buff[6]);

                if(res->sprites[currentAction] == NULL ){
                    reportErreur("Error malloc initEnemy():3");
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

void destroyEnemy(Enemy *p_enemy){
// Free enemy memory
    int i,j;
    for(i = 0; i < p_enemy->spritesSize1; i++){
        for(j = 0; j < p_enemy->spritesSize2[i]; j++){
            SDL_FreeSurface(p_enemy->sprites[i][j]);
        }
        free(p_enemy->sprites[i]);
        destroyCollider(p_enemy->actionColl[i]);
    }
    free(p_enemy->sprites);
    free(p_enemy->actionColl);

    free(p_enemy->speed);
    free(p_enemy->spriteDuration);
    free(p_enemy->spritesSize2);
    free(p_enemy->specialBehaviour);
    free(p_enemy);
}//------------------------------------------------------------------------------------------------------------------------

