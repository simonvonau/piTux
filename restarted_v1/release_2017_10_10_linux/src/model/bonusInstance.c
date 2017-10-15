#include "bonusInstance.h"


BonusInstance * initBonusInstance(int p_idBonus, int p_posX, int p_posY, Collider *p_coll){
// Init a BonusInstance
    BonusInstance *res = malloc(sizeof(BonusInstance));

    if(res == NULL){
        reportErreur("Error malloc() initBonusInstance()");
    }
    res->idBonus = p_idBonus;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->movementX = 0;
    res->movementY = 0;
    res->posX = p_posX;
    res->posY = p_posY;
    res->direction = -1;
    res->coll = p_coll;
    res->wasGathered = 0;
    res->lifeTime = 0;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void refreshBonusInstance(BonusInstance *p_bonusInstance, int p_loopTime, Bonus p_bonus){
// Refresh a bonusInstance
    int movementX, movementY;
    p_bonusInstance->currentTime += p_loopTime;

    // Specific behaviour of the coins (they go up after being gathered)
    if(p_bonusInstance->wasGathered && p_bonusInstance->coll->ownerTag == tag_bonus_coin){
        p_bonusInstance->lifeTime -= p_loopTime;
        p_bonusInstance->movementY += p_loopTime / 1000.0 * p_bonus.speedY;
        movementY = (int)p_bonusInstance->movementY;
        if(movementY >= 1 || movementY <= -1){
            p_bonusInstance->posY += movementY;
            p_bonusInstance->movementY -= movementY;
        }
    }

    // Specific behaviour of the egg (move horizontally + sensitive to gravity)
    if(p_bonusInstance->coll->ownerTag == tag_bonus_egg){
        p_bonusInstance->movementY -= p_loopTime / 1000.0 * GRAVITY_SPEED;
        p_bonusInstance->movementX += p_loopTime / 1000.0 * p_bonus.speedX * p_bonusInstance->direction;
        movementY = (int)p_bonusInstance->movementY;
        movementX = (int)p_bonusInstance->movementX;
        if(movementY >= 1 || movementY <= -1){
            p_bonusInstance->posY += movementY;
            p_bonusInstance->movementY -= movementY;
        }
        if(movementX >= 1 || movementX <= -1){
            p_bonusInstance->posX += movementX;
            p_bonusInstance->movementX -= movementX;
        }
    }

    // Specific behaviour of the flower (sensitive to gravity)
    if(p_bonusInstance->coll->ownerTag == tag_bonus_flower){
        p_bonusInstance->movementY += p_loopTime / 1000.0 * GRAVITY_SPEED;
        movementY = (int)p_bonusInstance->movementY;
        if(movementY >= 1 || movementY <= -1){
            p_bonusInstance->posY -= movementY;
            p_bonusInstance->movementY -= movementY;
        }
    }

    // Changing sprite
    if(p_bonusInstance->currentTime >= p_bonus.spriteDuration[p_bonusInstance->currentActionId]){
        p_bonusInstance->currentTime = 0;
        p_bonusInstance->currentSpriteId += 1;
        p_bonusInstance->currentSpriteId = p_bonusInstance->currentSpriteId % p_bonus.spritesSize2[p_bonusInstance->currentActionId];
    }
    p_bonusInstance->coll->lastPosX = p_bonusInstance->coll->posX;
    p_bonusInstance->coll->lastPosY = p_bonusInstance->coll->posY;
    p_bonusInstance->coll->posX = p_bonusInstance->posX;
    p_bonusInstance->coll->posY = p_bonusInstance->posY;

}//--------------------------------------------------------------------------------------------------------------------

void destroyBonusInstance(BonusInstance *p_bonusInstance){
//Free a bonusInstance
    destroyCollider(p_bonusInstance->coll);
    free(p_bonusInstance);
}//--------------------------------------------------------------------------------------------------------------------
