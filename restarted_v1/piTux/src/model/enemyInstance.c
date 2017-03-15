#include "enemyInstance.h"


EnemyInstance *initEnemyInstance(int idEnemy, int posX, int posY, Collider ** currColl, int collSize){
    EnemyInstance *res = malloc(sizeof(EnemyInstance));

    if(res == NULL){
        reportErreur("Error malloc() initEnemyInstance()");
    }

    res->idEnemy = idEnemy;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->movementProgressX = 0;
    res->movementProgressY = 0;
    res->posX = posX;
    res->posY = posY;
    res->isDead = 0;
    res->direction = -1; // Going left by default
    res->isFalling = 0;
    res->coll = currColl;
    res->collSize = collSize;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void refreshEnemyInstance(EnemyInstance *currEnemyInstance, int loopTime, Enemy currEnemy){
// Refresh an enemy
    int movementX, movementY;

    currEnemyInstance->currentTime += loopTime;
    // Changing sprite
    if(currEnemyInstance->currentTime >= currEnemy.spriteDuration[currEnemyInstance->currentActionId]){
        currEnemyInstance->currentTime = 0;
        currEnemyInstance->currentSpriteId += 1;
        currEnemyInstance->currentSpriteId = currEnemyInstance->currentSpriteId % currEnemy.spritesSize2[currEnemyInstance->currentActionId];
    }

    // Classic movement: go to left/right
    currEnemyInstance->movementProgressX += currEnemyInstance->direction * loopTime / 1000.0 * currEnemy.speed[currEnemyInstance->currentActionId];
    movementX = (int) currEnemyInstance->movementProgressX;
    if ( movementX >= 1 || movementX <= -1){
        currEnemyInstance->posX += movementX;
        currEnemyInstance->movementProgressX -= movementX;
    }
    // Gravity
    currEnemyInstance->movementProgressY -= loopTime / 1000.0 * GRAVITY_SPEED;
    movementY = (int) currEnemyInstance->movementProgressY;
    if ( movementY >= 1 || movementY <= -1){
        currEnemyInstance->posY += movementY;
        currEnemyInstance->movementProgressY -= movementY;
    }

    // Update old Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosX = currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY = currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY;

    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;

}//--------------------------------------------------------------------------------------------------------------------

void displayEnemyInstance(EnemyInstance *currEnemyInstance, Enemy currEnemy, SDL_Window *p_window, int deplaX, int deplaY){
// Display an enemy instance
    SDL_Rect objectPos = { 0, 0, 0, 0};
    objectPos.x = currEnemyInstance->posX + deplaX;
    objectPos.y = SDL_GetWindowSurface(p_window)->h - currEnemyInstance->posY
                - currEnemyInstance->coll[currEnemyInstance->currentActionId]->height
                - currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY * 2
                + deplaY;

    SDL_BlitSurface(currEnemy.sprites[currEnemyInstance->currentActionId][currEnemyInstance->currentSpriteId],
                         NULL, SDL_GetWindowSurface(p_window), &objectPos);

}//--------------------------------------------------------------------------------------------------------------------

void destroyEnemyInstance(EnemyInstance *currEnemyInstance){
// Free enemy memory
    int i;
    for(i = 0; i < currEnemyInstance->collSize; i++){
        destroyCollider(currEnemyInstance->coll[i]);
    }
    free(currEnemyInstance->coll);
    free(currEnemyInstance);
}//--------------------------------------------------------------------------------------------------------------------
