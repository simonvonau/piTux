#ifndef __COLLISION_H__
#define __COLLISION_H__
#include <stdlib.h>
#include <stdio.h>

#include "../utils/reporting.h"

typedef struct{
    int id;
    int coll1Id;
    int coll2Id;
}Collision;


Collision * initCollision(int id, int currColl1Id, int currColl2Id);
void destroyCollision(Collision* currCollision);


#endif
