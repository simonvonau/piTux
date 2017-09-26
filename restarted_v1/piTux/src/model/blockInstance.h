#ifndef __BLOCKINSTANCE_H__
#define __BLOCKINSTANCE_H__
#include <stdlib.h>
#include <stdio.h>


#include "../utils/reporting.h"
#include "../model/block.h"
#include "../model/collider.h"

typedef struct{
    int idBlock;
    int idBonus; // When this blockInstance contain something like a coin,...
    int currentTime;
    int currentActionId;
    int currentSpriteId;
    int posX, posY;
    Collider *coll;
    int wasDestroyed;
    int startPosY;
    float movementY;
}BlockInstance;


BlockInstance * initBlockInstance(int p_idBlock, int p_idBonus, int p_posX, int p_posY, Collider* p_coll);
void moveBlockInstance(BlockInstance *p_block, int p_moveX, int p_moveY);
void changeBlockAction(BlockInstance *p_block, int p_newActionId, int p_newSpriteId);
void refreshBlockInstance(BlockInstance * p_blockInstance, int p_loopTime, Block p_block);
void destroyBlockInstance(BlockInstance *p_blockInstance);


#endif
