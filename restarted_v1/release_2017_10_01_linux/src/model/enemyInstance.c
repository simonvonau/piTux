#include "enemyInstance.h"


EnemyInstance *initEnemyInstance(int p_idEnemy, int p_posX, int p_posY, Collider ** p_coll, int p_collSize){
    EnemyInstance *res = malloc(sizeof(EnemyInstance));

    if(res == NULL){
        reportErreur("Error malloc() initEnemyInstance()");
    }

    res->idEnemy = p_idEnemy;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->movementProgressX = 0;
    res->movementProgressY = 0;
    res->posX = p_posX;
    res->posY = p_posY;
    res->isDead = 0;
    res->lifeTime= 0;
    res->direction = -1; // Going to screen's left border by default
    res->isFalling = 0;
    res->isStuck = 0;
    res->isExposedToGravity = 1;
    res->coll = p_coll;
    res->collSize = p_collSize;
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
    p_enemyInstance->coll[p_newAction]->lastPosY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY;

    p_enemyInstance->currentActionId = p_newAction;
    p_enemyInstance->currentSpriteId = p_newSprite;
}//------------------------------------------------------------------------------------------------------------------------

void refreshEnemyInstance(EnemyInstance *p_enemyInstance, int p_loopTime, Enemy p_enemy){
// Refresh an enemy
    int movementX, movementY;

    p_enemyInstance->currentTime += p_loopTime;
    p_enemyInstance->lifeTime -= p_loopTime;

    // Changing sprite
    if(p_enemyInstance->currentTime >= p_enemy.spriteDuration[p_enemyInstance->currentActionId]){
        p_enemyInstance->currentTime = 0;
        p_enemyInstance->currentSpriteId += 1;
        p_enemyInstance->currentSpriteId = p_enemyInstance->currentSpriteId % p_enemy.spritesSize2[p_enemyInstance->currentActionId];
    }

    // Classic movement: go to left/right
    if(!p_enemyInstance->isDead && !p_enemyInstance->isStuck){
        p_enemyInstance->movementProgressX += p_enemyInstance->direction * p_loopTime / 1000.0 * p_enemy.speed[p_enemyInstance->currentActionId];
        movementX = (int) p_enemyInstance->movementProgressX;
        if ( movementX >= 1 || movementX <= -1){
            p_enemyInstance->posX += movementX;
            p_enemyInstance->movementProgressX -= movementX;
        }
    }
    // Gravity
    if(p_enemyInstance->isExposedToGravity){
        p_enemyInstance->movementProgressY -= p_loopTime / 1000.0 * GRAVITY_SPEED;
        movementY = (int) p_enemyInstance->movementProgressY;
        if ( movementY >= 1 || movementY <= -1){
            p_enemyInstance->posY += movementY;
            p_enemyInstance->movementProgressY -= movementY;
        }
    }

    // Update old Collider position
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosX = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX;
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY;
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX = p_enemyInstance->posX;
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY = p_enemyInstance->posY;

}//--------------------------------------------------------------------------------------------------------------------

void displayEnemyInstance(EnemyInstance *p_enemyInstance, Enemy p_enemy, SDL_Window *p_window, int p_deplaX, int p_deplaY){
// Display an enemy instance
    SDL_Rect objectPos = { 0, 0, 0, 0};
    objectPos.x = p_enemyInstance->posX + p_deplaX;
    objectPos.y = SDL_GetWindowSurface(p_window)->h - p_enemyInstance->posY
                - p_enemyInstance->coll[p_enemyInstance->currentActionId]->height
                - p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY * 2
                + p_deplaY;

    SDL_BlitSurface(p_enemy.sprites[p_enemyInstance->currentActionId][p_enemyInstance->currentSpriteId],
                         NULL, SDL_GetWindowSurface(p_window), &objectPos);

}//--------------------------------------------------------------------------------------------------------------------

void destroyEnemyInstance(EnemyInstance *p_enemyInstance){
// Free enemy memory
    int i;
    for(i = 0; i < p_enemyInstance->collSize; i++){
        destroyCollider(p_enemyInstance->coll[i]);
    }
    free(p_enemyInstance->coll);
    free(p_enemyInstance);
}//--------------------------------------------------------------------------------------------------------------------
