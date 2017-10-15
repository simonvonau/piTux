#ifndef __COLLIDERMANAGER_H__
#define __COLLIDERMANAGER_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy


#include "../utils/reporting.h"
#include "../model/collider.h"
#include "../model/collision.h"


typedef struct{
    Collider **allColliders;
    int allCollidersSize;
    Collision **allCollisions;
    int allCollisionsSize;
}ColliderManager;


ColliderManager * initColliderManager();

void addColliderToColliderManager(ColliderManager *p_colliderManager, Collider *p_coll);
Collider * createCollider(ColliderManager *p_colliderManager, int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, int p_ownerTag);
Collider * colliderDeepCopyByColliderManager(ColliderManager *p_colliderManager, Collider *p_coll);
Collider ** colliderArrayDeepCopyByColliderManager(ColliderManager *p_colliderManager, Collider **p_coll, int p_size);
int removeCollider(ColliderManager *p_colliderManager, int p_idCollider, int p_freeMemory);

Collider * getColliderById(ColliderManager *p_collMgr, int p_id);
void createCollision(ColliderManager *p_collMgr, int p_coll1Id, int p_coll2Id);
int getNextAvailableColliderId(ColliderManager *p_colliderManager);
void updateCollisions(ColliderManager *p_colliderManager, int p_leftLimit, int p_rightLimit, int p_bottomLimit, int p_topLimit);
void getColliderTouching(ColliderManager *p_collMgr,int p_colliderId, Collider ***p_res, int *p_resSize);

void destroyColliderManager(ColliderManager *p_colliderManager);


#endif
