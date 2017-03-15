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
}BlockInstance;


BlockInstance * initBlockInstance(int idBlock, int idBonus, int posX, int posY, Collider* coll);
void refreshBlockInstance(BlockInstance * currBlockInstance, int loopTime, Block currBlock);
void destroyBlockInstance(BlockInstance *currBlockInstance);


#endif
