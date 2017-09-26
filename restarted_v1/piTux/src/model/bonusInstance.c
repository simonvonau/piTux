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
    if(p_bonusInstance->wasGathered && p_bonusInstance->coll->ownerTag == TAG_BONUS_COIN){
        p_bonusInstance->lifeTime -= p_loopTime;
        p_bonusInstance->movementY += p_loopTime / 1000.0 * p_bonus.speedY;
        movementY = (int)p_bonusInstance->movementY;
        if(movementY >= 1 || movementY <= -1){
            p_bonusInstance->posY += movementY;
            p_bonusInstance->movementY -= movementY;
        }
    }

    // Specific behiour of the egg (move horizontally)
    /*if(p_bonusInstance->coll->ownerTag == TAG_BONUS_EGG){
        p_bonusInstance->movementX += p_loopTime / 1000.0 * p_bonus.speedX;
        movementX = (int)p_bonusInstance->movementX;
        if(movementX >= 1 || movementX <= -1){
            p_bonusInstance->posX += movementX;
            p_bonusInstance->movementX -= movementX;
        }
    }*/

    // Changing sprite
    if(p_bonusInstance->currentTime >= p_bonus.spriteDuration[p_bonusInstance->currentActionId]){
        p_bonusInstance->currentTime = 0;
        p_bonusInstance->currentSpriteId += 1;
        p_bonusInstance->currentSpriteId = p_bonusInstance->currentSpriteId % p_bonus.spritesSize2[p_bonusInstance->currentActionId];
    }
}//--------------------------------------------------------------------------------------------------------------------

void destroyBonusInstance(BonusInstance *p_bonusInstance){
//Free a bonusInstance
    destroyCollider(p_bonusInstance->coll);
    free(p_bonusInstance);
}//--------------------------------------------------------------------------------------------------------------------
