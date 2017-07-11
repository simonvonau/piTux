#include "bonusInstance.h"


BonusInstance * initBonusInstance(int idBonus, int posX, int posY, Collider *coll){
// Init a BonusInstance
    BonusInstance *res = malloc(sizeof(BonusInstance));

    if(res == NULL){
        reportErreur("Error malloc() initBonusInstance()");
    }
    res->idBonus = idBonus;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->movementX = 0;
    res->movementY = 0;
    res->posX = posX;
    res->posY = posY;
    res->coll = coll;
    res->wasGathered = 0;
    res->lifeTime = 0;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void refreshBonusInstance(BonusInstance *currBonusInstance, int loopTime, Bonus currBonus){
// Refresh a bonusInstance
    int movementX, movementY;
    currBonusInstance->currentTime += loopTime;

    // Specific behaviour of the coins (they go up after being gathered)
    if(currBonusInstance->wasGathered && currBonusInstance->coll->ownerTag == TAG_BONUS_COIN){
        currBonusInstance->lifeTime -= loopTime;
        currBonusInstance->movementY += loopTime / 1000.0 * currBonus.speedY;
        movementY = (int)currBonusInstance->movementY;
        if(movementY >= 1 || movementY <= -1){
            currBonusInstance->posY += movementY;
            currBonusInstance->movementY -= movementY;
        }
    }

    // Specific behiour of the egg (move horizontally)
    /*if(currBonusInstance->coll->ownerTag == TAG_BONUS_EGG){
        currBonusInstance->movementX += loopTime / 1000.0 * currBonus.speedX;
        movementX = (int)currBonusInstance->movementX;
        if(movementX >= 1 || movementX <= -1){
            currBonusInstance->posX += movementX;
            currBonusInstance->movementX -= movementX;
        }
    }*/

    // Changing sprite
    if(currBonusInstance->currentTime >= currBonus.spriteDuration[currBonusInstance->currentActionId]){
        currBonusInstance->currentTime = 0;
        currBonusInstance->currentSpriteId += 1;
        currBonusInstance->currentSpriteId = currBonusInstance->currentSpriteId % currBonus.spritesSize2[currBonusInstance->currentActionId];
    }
}//--------------------------------------------------------------------------------------------------------------------

void destroyBonusInstance(BonusInstance *currBonusInstance){
//Free a bonusInstance
    destroyCollider(currBonusInstance->coll);
    free(currBonusInstance);
}//--------------------------------------------------------------------------------------------------------------------
