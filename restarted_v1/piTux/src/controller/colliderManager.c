#include "colliderManager.h"

ColliderManager * initColliderManager(){
// Init a new colliderManager
    ColliderManager *res = malloc(sizeof(ColliderManager));
    if (res == NULL){
        reportErreur("colliderManager:initColliderManager(...): error malloc(..)");
    }
    res->allColliders = NULL;
    res->allCollidersSize = 0;
    res->allCollisions = NULL;
    res->allCollisionsSize = 0;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void addColliderToColliderManager(ColliderManager *p_colliderManager, Collider *p_coll){
// Add Collider to the ColliderManager Array
    Collider * temp[p_colliderManager->allCollidersSize];

    // Save, reallocate and back up data
    if (p_colliderManager->allColliders == NULL || p_colliderManager->allCollidersSize == 0){
        p_colliderManager->allColliders = malloc(sizeof(Collider *));
        if (p_colliderManager->allColliders == NULL){
            reportErreur("colliderManager:addColliderToColliderManager(...): error malloc()");
        }
    }else{
        memcpy(temp, p_colliderManager->allColliders, p_colliderManager->allCollidersSize * sizeof(Collider *));
        //*** realloc() doesn't work... weird...
        //realloc(p_colliderManager->allColliders, (p_colliderManager->allCollidersSize + 1) * sizeof(Collider *));
        free(p_colliderManager->allColliders);
        p_colliderManager->allColliders = malloc((p_colliderManager->allCollidersSize + 1) * sizeof(Collider *));
        if (p_colliderManager->allColliders == NULL){
            reportErreur("colliderManager:addColliderToColliderManager(...): error realloc()");
        }
        memcpy(p_colliderManager->allColliders, temp, p_colliderManager->allCollidersSize * sizeof(Collider *));
    }
    // Add the new Collider:
    p_colliderManager->allCollidersSize += 1;
    p_colliderManager->allColliders[p_colliderManager->allCollidersSize - 1] = p_coll;

}//------------------------------------------------------------------------------------------------------------------------

Collider * createCollider(ColliderManager *p_colliderManager, int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, int p_ownerTag){
// Create a new collider and add it to a colliderManager
    Collider *res = initCollider(getNextAvailableColliderId(p_colliderManager), p_width, p_height, p_posX, p_posY, p_isEnabled, p_ownerTag, state_normal);
    addColliderToColliderManager(p_colliderManager, res);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider * colliderDeepCopyByColliderManager(ColliderManager *p_colliderManager, Collider *p_coll){
// Make a deep copy (=create a different reference) from a Collider and add it to a colliderManager
    Collider *res = NULL;
    if (p_coll != NULL){
        res = malloc(sizeof(Collider));
        if (res == NULL){
            reportErreur("Error malloc colliderDeepCopy()");
        }
        memcpy(res, p_coll, sizeof(Collider));
        res->id = getNextAvailableColliderId(p_colliderManager);
        addColliderToColliderManager(p_colliderManager, res);
    }
    //printf("CollMgr: added: %d \n", res->id);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider ** colliderArrayDeepCopyByColliderManager(ColliderManager *p_colliderManager, Collider **p_coll, int p_size){
// Make a deep copy (=create a different reference) from a Collider array and add it to a colliderManager
    Collider **res = NULL;
    int i;
    if (p_coll != NULL && p_size != 0){
        res = malloc(p_size * sizeof(Collider *));
        if (res == NULL){
            reportErreur("Error malloc colliderArrayDeepCopy()");
        }
        for(i = 0; i < p_size; i++){
            res[i] = colliderDeepCopyByColliderManager(p_colliderManager, p_coll[i]);
        }
    }
    return res;
}//------------------------------------------------------------------------------------------------------------------------

int removeCollider(ColliderManager *p_colliderManager, int p_idCollider, int p_freeMemory){
// Destroy a collider
    int i;
    int wasFounded = 0;
    Collider *tempArray[p_colliderManager->allCollidersSize];

    if(p_colliderManager->allCollidersSize == 0){return 0;}
    for(i = 0; i < p_colliderManager->allCollidersSize; i++){
        if(p_colliderManager->allColliders[i]->id == p_idCollider){
            if (wasFounded){
                reportErreur("destroyCollider(): Error multiple occurence of this ID");
            }
            wasFounded += 1;
        }
        // Put the collider to delete at the end
        if (wasFounded && i < p_colliderManager->allCollidersSize - 1){
            p_colliderManager->allColliders[i] = p_colliderManager->allColliders[i + 1];
        }
    }
    p_colliderManager->allCollidersSize -= 1;
    memcpy(tempArray, p_colliderManager->allColliders, sizeof(Collider *) * (p_colliderManager->allCollidersSize + 1));
    free(p_colliderManager->allColliders);
    p_colliderManager->allColliders = malloc(sizeof(Collider *) * p_colliderManager->allCollidersSize);
    memcpy(p_colliderManager->allColliders, tempArray, sizeof(Collider *) * p_colliderManager->allCollidersSize);

    // In most of the case the memory is deallocated from the hero/enemy/bonus Instance object
    if(p_freeMemory){
        destroyCollider(tempArray[p_colliderManager->allCollidersSize]);
    }

    if(p_colliderManager->allColliders == NULL){reportErreur("colliderManager:destroyCollider(...): error malloc()");}

    //*** Realloc doesn't work... (weird)
    /*if( realloc(p_colliderManager->allColliders, (p_colliderManager->allCollidersSize - 1) * sizeof(Collider *)) == NULL){
        reportErreur("colliderManager:destroyCollider(...): error realloc()");
    }*/

    return wasFounded;
}//------------------------------------------------------------------------------------------------------------------------

Collider * getColliderById(ColliderManager *p_collMgr, int p_id){
// Get a Collider by it's Id
    int i;
    for(i = 0; i < p_collMgr->allCollidersSize; i++){
        if( p_collMgr->allColliders[i]->id == p_id){
            return p_collMgr->allColliders[i];
        }
    }
    reportErreur("colliderManager.c:getColliderById(): cannot find collider with given Id");
    return NULL;
}//------------------------------------------------------------------------------------------------------------------------

void createCollision(ColliderManager *p_collMgr, int p_coll1Id, int p_coll2Id){
// Create a new Collision and add it to the manager array
    int i,j, isAvailable, availableId, isCollisionAlreadyExists;
    Collision *temp [p_collMgr->allCollisionsSize];

    // Test if this Collision already exists
    isCollisionAlreadyExists = 0;
    for(i = 0; i < p_collMgr->allCollisionsSize; i++){
        if ((p_collMgr->allCollisions[i]->coll1Id == p_coll1Id && p_collMgr->allCollisions[i]->coll2Id == p_coll2Id)
        ||(p_collMgr->allCollisions[i]->coll1Id == p_coll2Id && p_collMgr->allCollisions[i]->coll2Id == p_coll1Id)){
            isCollisionAlreadyExists = 1;
            break;
        }
    }

    // Find a available Id
    for(i = 0; !isCollisionAlreadyExists && i < p_collMgr->allCollisionsSize - 1 ; i++){
        isAvailable = 1;
        for(j = 0; j < p_collMgr->allCollisionsSize - 1; j++){
            if (p_collMgr->allCollisions[j]->id == i){
                isAvailable = 0;
            }
        }
        if (isAvailable){
            break;
        }
    }
    if (isAvailable){
        availableId = i;
    }else{
        availableId = p_collMgr->allCollisionsSize - 1;
    }

    // Add 1 field to the Collisions array and fill it
    if (!isCollisionAlreadyExists){
        if( p_collMgr->allCollisions == NULL || p_collMgr->allCollisionsSize == 0){
            p_collMgr->allCollisions = malloc(sizeof(Collision*));
            if (p_collMgr->allCollisions == NULL){reportErreur("colliderManager:createCollision(...): error malloc()1");}
        }else{
            memcpy(temp, p_collMgr->allCollisions, p_collMgr->allCollisionsSize * sizeof(Collision*));
            //*** realloc doesn't work...
                //realloc(p_collMgr->allCollisions, (p_collMgr->allCollisionsSize + 1) * sizeof(Collision*));
            free(p_collMgr->allCollisions);
            p_collMgr->allCollisions = malloc((p_collMgr->allCollisionsSize + 1) * sizeof(Collision*));
            if (p_collMgr->allCollisions == NULL){reportErreur("colliderManager:createCollision(...): error malloc()2");}
            memcpy(p_collMgr->allCollisions, temp, p_collMgr->allCollisionsSize * sizeof(Collision*));
        }
        p_collMgr->allCollisionsSize += 1;
        // Create the new Collision
        p_collMgr->allCollisions[p_collMgr->allCollisionsSize - 1] = initCollision(availableId, p_coll1Id, p_coll2Id);

    }
}//------------------------------------------------------------------------------------------------------------------------

int getNextAvailableColliderId(ColliderManager *p_colliderManager){
// Get the next available id for a collider
    int i, j, isAvailable;
    for(i = 0; i < p_colliderManager->allCollidersSize; i++){
        isAvailable = 1;
        for(j = 0; j < p_colliderManager->allCollidersSize; j++){
            if(p_colliderManager->allColliders[j]->id == i){
                isAvailable = 0;
                break;
            }
        }
        if (isAvailable){
            return i;
        }
    }
    return p_colliderManager->allCollidersSize;
}//------------------------------------------------------------------------------------------------------------------------

void updateCollisions(ColliderManager *p_colliderManager, int p_leftLimit, int p_rightLimit, int p_bottomLimit, int p_topLimit){
// Update all the collisions between each collider
    int i, j;
    int temp;
    // Clearing previous Collisions
    if (p_colliderManager->allCollisions != NULL){
        for(i = 0; i < p_colliderManager->allCollisionsSize; i++){
            destroyCollision(p_colliderManager->allCollisions[i]);
        }
        free(p_colliderManager->allCollisions);
        p_colliderManager->allCollisions = NULL;
        p_colliderManager->allCollisionsSize = 0;
    }


    for(i = 0; i < p_colliderManager->allCollidersSize; i++){
        //*** Temporary
        /*if (p_colliderManager->allColliders[i]->isEnabled){
            printf("collider enabled (x, y, w, h): %d,%d,%d,%d \n",
                   p_colliderManager->allColliders[i]->posX, p_colliderManager->allColliders[i]->posY
                   , p_colliderManager->allColliders[i]->width, p_colliderManager->allColliders[i]->height);
        }*/
        // Only takes collider enabled and inside the screen (leftLimit, rightLimit, bottomLimit, topLimit) to decrease the processing time
        if (p_colliderManager->allColliders[i]->isEnabled
        && p_colliderManager->allColliders[i]->posX < p_rightLimit
        && p_colliderManager->allColliders[i]->posX + p_colliderManager->allColliders[i]->width > p_leftLimit
        && p_colliderManager->allColliders[i]->posY < p_topLimit
        && p_colliderManager->allColliders[i]->posY + p_colliderManager->allColliders[i]->height > p_bottomLimit){
            for(j = 0; j < p_colliderManager->allCollidersSize; j++){
                if ( i != j && p_colliderManager->allColliders[j]->isEnabled){

                    if(p_colliderManager->allColliders[i]->posX < p_colliderManager->allColliders[j]->posX + p_colliderManager->allColliders[j]->width
                    && p_colliderManager->allColliders[i]->posX + p_colliderManager->allColliders[i]->width > p_colliderManager->allColliders[j]->posX){
                        if(p_colliderManager->allColliders[i]->posY < p_colliderManager->allColliders[j]->posY + p_colliderManager->allColliders[j]->height
                        && p_colliderManager->allColliders[i]->posY + p_colliderManager->allColliders[i]->height > p_colliderManager->allColliders[j]->posY){
                            createCollision(p_colliderManager, p_colliderManager->allColliders[i]->id, p_colliderManager->allColliders[j]->id);
                        }
                    }
                }
            }// for end
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void getColliderTouching(ColliderManager *p_collMgr,int p_colliderId, Collider ***p_res, int *p_resSize){
// Find Colliders which collides with given id Collider
    int i;
    Collider **tempArray;
    *p_res = malloc(p_collMgr->allCollisionsSize * sizeof(Collider *));
    *p_resSize = 0;
    if (p_res == NULL){
        reportErreur("colliderManager.c:getColliderTouching(): error malloc()");
    }

    for(i = 0; i < p_collMgr->allCollisionsSize; i++){
        // If one of the 2 colliders has the right id
        if(p_collMgr->allCollisions[i]->coll1Id == p_colliderId){
            (*p_res)[*p_resSize] = getColliderById(p_collMgr, p_collMgr->allCollisions[i]->coll2Id);
            *p_resSize += 1;
        }else if(p_collMgr->allCollisions[i]->coll2Id == p_colliderId){
            (*p_res)[*p_resSize] = getColliderById(p_collMgr, p_collMgr->allCollisions[i]->coll1Id);
            *p_resSize += 1;
        }
    }
    // Reduce p_res allocated memory by allocating a smaller array
    if(*p_resSize < p_collMgr->allCollisionsSize){
        tempArray = malloc(*p_resSize * sizeof(Collider *));
        if(tempArray == NULL){reportErreur("colliderManager.c:getColliderTouching(): error malloc2()");}
        memcpy(tempArray, *p_res, *p_resSize * sizeof(Collider *));
        free(*p_res);
        *p_res = tempArray;
    }
}//------------------------------------------------------------------------------------------------------------------------

void destroyColliderManager(ColliderManager *p_colliderManager){
// Free colliderManager memory
    int i;
    for(i = 0; i < p_colliderManager->allCollidersSize; i++){
        destroyCollider(p_colliderManager->allColliders[i]);
    }
    free(p_colliderManager->allColliders);

    for(i = 0; i < p_colliderManager->allCollisionsSize; i++){
        destroyCollision(p_colliderManager->allCollisions[i]);
    }
    free(p_colliderManager->allCollisions);
}//------------------------------------------------------------------------------------------------------------------------

