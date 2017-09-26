#include "blockInstance.h"


BlockInstance * initBlockInstance(int p_idBlock, int p_idBonus, int p_posX, int p_posY, Collider* p_coll){
// Create a new blockInstance
    BlockInstance *res = malloc(sizeof(BlockInstance));

    if(res == NULL){
        reportErreur("Error malloc() initBlockInstance()");
    }
    res->idBlock = p_idBlock;
    res->idBonus = p_idBonus;
    res->posX = p_posX;
    res->posY = p_posY;
    res->currentTime = 0;
    res->currentActionId = 0;
    res->currentSpriteId = 0;
    res->coll = p_coll;
    res->wasDestroyed = 0;
    res->startPosY = res->posY;
    res->movementY = 0;
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void moveBlockInstance(BlockInstance *p_block, int p_moveX, int p_moveY){
    // Move a blockInstance
    p_block->coll->posX += p_moveX;
    p_block->coll->posY += p_moveY;
    p_block->posX += p_moveX;
    p_block->posY += p_moveY;
}//--------------------------------------------------------------------------------------------------------------------

void changeBlockAction(BlockInstance *p_block, int p_newActionId, int p_newSpriteId){
    // Change the blockInstance's sprite
    if(p_block->currentActionId != p_newActionId){
        p_block->currentActionId = p_newActionId;
        p_block->currentSpriteId = p_newSpriteId;
        p_block->currentTime = 0;
    }
}//--------------------------------------------------------------------------------------------------------------------

void refreshBlockInstance(BlockInstance * p_blockInstance, int p_loopTime, Block p_block){
// Refresh a blockInstance
    p_blockInstance->currentTime += p_loopTime;

    // Wood block destruction
    if(p_blockInstance->coll->ownerTag == TAG_BLOCK_UNDERHIT_WEAK){
        p_blockInstance->movementY += p_block.speedY * p_loopTime / 1000.0;
        if(p_blockInstance->movementY >= 1 || p_blockInstance->movementY <= -1){
            moveBlockInstance(p_blockInstance, 0, (int)p_blockInstance->movementY);
            p_blockInstance->movementY = 0;
        }

        if(p_blockInstance->currentTime >= p_block.spriteDuration[p_blockInstance->currentActionId]){
            p_blockInstance->currentTime = 0;
            p_blockInstance->currentSpriteId += 1;
        }
        if(p_blockInstance->currentSpriteId >= p_block.spritesSize2[p_blockInstance->currentActionId]){
            p_blockInstance->wasDestroyed = 1;
            p_blockInstance->coll->isEnabled = 0;
        }
        p_blockInstance->currentSpriteId = p_blockInstance->currentSpriteId % p_block.spritesSize2[p_blockInstance->currentActionId];
    }

    // "?" block opening when going down
    if(p_blockInstance->coll->ownerTag == TAG_BLOCK_UNDERHIT_NORMAL_DESC){
        p_blockInstance->movementY -= p_block.speedY * p_loopTime / 1000.0;
        if(p_blockInstance->movementY >= 1 || p_blockInstance->movementY <= -1){
            moveBlockInstance(p_blockInstance, 0, (int)p_blockInstance->movementY);
            p_blockInstance->movementY = 0;
        }
        if(p_blockInstance->posY <= p_blockInstance->startPosY){
            moveBlockInstance(p_blockInstance, 0, p_blockInstance->startPosY - p_blockInstance->posY);
            p_blockInstance->coll->ownerTag = TAG_BLOCK_NORMAL;
            p_blockInstance->movementY = 0;
        }
        if(p_blockInstance->currentTime >= p_block.spriteDuration[p_blockInstance->currentActionId]){
            p_blockInstance->currentTime = 0;
            p_blockInstance->currentSpriteId += 1;
        }
        p_blockInstance->currentSpriteId = p_blockInstance->currentSpriteId % p_block.spritesSize2[p_blockInstance->currentActionId];
    }
    // "?" block opening when going up
    if(p_blockInstance->coll->ownerTag == TAG_BLOCK_UNDERHIT_NORMAL){
        p_blockInstance->movementY += p_block.speedY * p_loopTime / 1000.0;
        if(p_blockInstance->movementY >= 1 || p_blockInstance->movementY <= -1){
            moveBlockInstance(p_blockInstance, 0, (int)p_blockInstance->movementY);
            p_blockInstance->movementY = 0;
        }
        if(p_blockInstance->currentTime >= p_block.spriteDuration[p_blockInstance->currentActionId]){
            p_blockInstance->currentTime = 0;
            p_blockInstance->currentSpriteId += 1;
        }
        if(p_blockInstance->currentSpriteId >= p_block.spritesSize2[p_blockInstance->currentActionId]){
            p_blockInstance->coll->ownerTag = TAG_BLOCK_UNDERHIT_NORMAL_DESC;
        }
        p_blockInstance->currentSpriteId = p_blockInstance->currentSpriteId % p_block.spritesSize2[p_blockInstance->currentActionId];
    }
}//--------------------------------------------------------------------------------------------------------------------

void destroyBlockInstance(BlockInstance *p_blockInstance){
// Free a blockInstance
    destroyCollider(p_blockInstance->coll);
    free(p_blockInstance);
}//--------------------------------------------------------------------------------------------------------------------
