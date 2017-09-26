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


Collision * initCollision(int p_id, int p_coll1Id, int p_coll2Id);
void destroyCollision(Collision* p_collision);


#endif
