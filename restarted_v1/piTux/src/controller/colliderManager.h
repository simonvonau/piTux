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

void addColliderToColliderManager(ColliderManager *currColliderManager, Collider *currColl);
Collider * createCollider(ColliderManager *currColliderManager, int width, int height, int posX, int posY, int isEnabled, int ownerTag);
Collider * colliderDeepCopyByColliderManager(ColliderManager *currColliderManager, Collider *coll);
Collider ** colliderArrayDeepCopyByColliderManager(ColliderManager *currColliderManager, Collider **coll, int currSize);
int removeCollider(ColliderManager *currColliderManager, int idCollider);
Collider * getColliderById(ColliderManager *collMgr, int id);
void createCollision(ColliderManager *currCollMgr, int coll1Id, int coll2Id);
int getNextAvailableColliderId(ColliderManager *currColliderManager);
void updateCollisions(ColliderManager *currColliderManager, int leftLimit, int rightLimit, int bottomLimit, int topLimit);
void getColliderTouching(ColliderManager *collMgr,int colliderId, Collider ***res, int *resSize);

void destroyColliderManager(ColliderManager *currColliderManager);


#endif
