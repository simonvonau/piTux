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

void changeEnemyAction(EnemyInstance * p_enemyInstance, int p_newAction, int p_newSprite, int *deltaX, int *deltaY){
    // Change the enemyInstance current action
    Collider *newColl = p_enemyInstance->coll[p_newAction];
    Collider *oldColl = p_enemyInstance->coll[p_enemyInstance->currentActionId];

    if(p_newAction == p_enemyInstance->currentActionId){return;}
    oldColl->isEnabled = 0;

    // If the new collider is smaller than the previous one (mostly use when the enemy die)
    *deltaX = 0;
    *deltaY = newColl->height - oldColl->height;
    if(*deltaY > 0){*deltaY = 0;}

    newColl->posX = oldColl->posX;
    newColl->posY = oldColl->posY + *deltaY;
    newColl->lastPosX = oldColl->lastPosX;
    newColl->lastPosY = oldColl->lastPosY;
    newColl->ownerTag = oldColl->ownerTag;

    p_enemyInstance->currentActionId = p_newAction;
    p_enemyInstance->currentSpriteId = p_newSprite;
    newColl->isEnabled = 1;
}//------------------------------------------------------------------------------------------------------------------------

void refreshEnemyInstance(EnemyInstance *p_enemyInstance, int p_loopTime, Enemy p_enemy){
// Refresh an enemy
    int movementX, movementY;

    Collider *eneColl = p_enemyInstance->coll[p_enemyInstance->currentActionId];

    eneColl->lastPosX = eneColl->posX;
    eneColl->lastPosY = eneColl->posY;

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
            eneColl->posX += movementX;
            p_enemyInstance->movementProgressX -= movementX;
        }
    }
    // Gravity
    if(p_enemyInstance->isExposedToGravity){
        p_enemyInstance->movementProgressY -= p_loopTime / 1000.0 * GRAVITY_SPEED;
        movementY = (int) p_enemyInstance->movementProgressY;
        if ( movementY >= 1 || movementY <= -1){
            eneColl->posY += movementY;
            p_enemyInstance->movementProgressY -= movementY;
        }
    }

    updEneSpriteLocFromCollLoc(p_enemyInstance, &p_enemy);
}//--------------------------------------------------------------------------------------------------------------------

void updEneSpriteLocFromCollLoc(EnemyInstance *p_enemyInstance, Enemy *p_enemy){
// Update the enemy sprite location using its collider location
    Collider *eneColl = p_enemyInstance->coll[p_enemyInstance->currentActionId];
    SDL_Surface *eneSprite = p_enemy->sprites[p_enemyInstance->currentActionId][p_enemyInstance->currentSpriteId];

    int decalX = (eneSprite->w - eneColl->width) / 2;
    int decalY = 0;

    p_enemyInstance->posX = eneColl->posX - decalX;
    p_enemyInstance->posY = eneColl->posY + decalY;

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
