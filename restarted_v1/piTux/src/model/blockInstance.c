#include "blockInstance.h"


BlockInstance * initBlockInstance(int idBlock, int idBonus, int posX, int posY, Collider* coll){
// Create a new blockInstance
    BlockInstance *res = malloc(sizeof(BlockInstance));

    if(res == NULL){
        reportErreur("Error malloc() initBlockInstance()");
    }
    res->idBlock = idBlock;
    res->idBonus = idBonus;
    res->posX = posX;
    res->posY = posY;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->coll = coll;
    return res;
}//--------------------------------------------------------------------------------------------------------------------


void refreshBlockInstance(BlockInstance * currBlockInstance, int loopTime, Block currBlock){
// Refresh a blockInstance
    currBlockInstance->currentTime += loopTime;
    // Changing sprite
    if(currBlockInstance->currentTime >= currBlock.spriteDuration[currBlockInstance->currentActionId]){
        currBlockInstance->currentTime = 0;
        currBlockInstance->currentSpriteId += 1;
        currBlockInstance->currentSpriteId = currBlockInstance->currentSpriteId % currBlock.spritesSize2[currBlockInstance->currentActionId];
    }

}//--------------------------------------------------------------------------------------------------------------------

void destroyBlockInstance(BlockInstance *currBlockInstance){
// Free a blockInstance
    destroyCollider(currBlockInstance->coll);
    free(currBlockInstance);
}//--------------------------------------------------------------------------------------------------------------------
