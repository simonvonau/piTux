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
    res->posX = posX;
    res->posY = posY;
    res->coll = coll;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void refreshBonusInstance(BonusInstance *currBonusInstance, int loopTime, Bonus currBonus){
// Refresh a bonusInstance
    currBonusInstance->currentTime += loopTime;
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
