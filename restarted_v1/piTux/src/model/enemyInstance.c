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
    res->lifeTime= 0;
    res->direction = -1; // Going to screen's left border by default
    res->isFalling = 0;
    res->isStuck = 0;
    res->isExposedToGravity = 1;
    res->coll = currColl;
    res->collSize = collSize;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void changeEnemyAction(EnemyInstance * p_enemyInstance, int p_newAction, int p_newSprite){
    // Change the enemyInstance current action
    int colliderYDiff;// Size difference between the new and the old collider
    if(p_newAction == p_enemyInstance->currentActionId){return;}
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->isEnabled = 0;
    p_enemyInstance->coll[p_newAction]->isEnabled = 1;

    // If the new collider is smaller than the previous one (mostly use when the enemy die)
    colliderYDiff = p_enemyInstance->coll[p_newAction]->height - p_enemyInstance->coll[p_enemyInstance->currentActionId]->height;
    if(colliderYDiff > 0){colliderYDiff = 0;}

    p_enemyInstance->posY += colliderYDiff;

    p_enemyInstance->coll[p_newAction]->posX = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX;
    p_enemyInstance->coll[p_newAction]->posY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY + colliderYDiff;
    p_enemyInstance->coll[p_newAction]->lastPosX = p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosX;
    p_enemyInstance->coll[p_newAction]->lastPosY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY + colliderYDiff;

    p_enemyInstance->currentActionId = p_newAction;
    p_enemyInstance->currentSpriteId = p_newSprite;
}//------------------------------------------------------------------------------------------------------------------------

void refreshEnemyInstance(EnemyInstance *currEnemyInstance, int loopTime, Enemy currEnemy){
// Refresh an enemy
    int movementX, movementY;

    currEnemyInstance->currentTime += loopTime;
    currEnemyInstance->lifeTime -= loopTime;

    // Changing sprite
    if(currEnemyInstance->currentTime >= currEnemy.spriteDuration[currEnemyInstance->currentActionId]){
        currEnemyInstance->currentTime = 0;
        currEnemyInstance->currentSpriteId += 1;
        currEnemyInstance->currentSpriteId = currEnemyInstance->currentSpriteId % currEnemy.spritesSize2[currEnemyInstance->currentActionId];
    }

    // Classic movement: go to left/right
    if(!currEnemyInstance->isDead && !currEnemyInstance->isStuck){
        currEnemyInstance->movementProgressX += currEnemyInstance->direction * loopTime / 1000.0 * currEnemy.speed[currEnemyInstance->currentActionId];
        movementX = (int) currEnemyInstance->movementProgressX;
        if ( movementX >= 1 || movementX <= -1){
            currEnemyInstance->posX += movementX;
            currEnemyInstance->movementProgressX -= movementX;
        }
    }
    // Gravity (only for the enemies with an active collider
    if(currEnemyInstance->isExposedToGravity){
        currEnemyInstance->movementProgressY -= loopTime / 1000.0 * GRAVITY_SPEED;
        movementY = (int) currEnemyInstance->movementProgressY;
        if ( movementY >= 1 || movementY <= -1){
            currEnemyInstance->posY += movementY;
            currEnemyInstance->movementProgressY -= movementY;
        }
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
