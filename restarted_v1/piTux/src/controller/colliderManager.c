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

void addColliderToColliderManager(ColliderManager *currColliderManager, Collider *currColl){
// Add Collider to the ColliderManager Array
    Collider * temp[currColliderManager->allCollidersSize];

    // Save, reallocate and back up data
    if (currColliderManager->allColliders == NULL || currColliderManager->allCollidersSize == 0){
        currColliderManager->allColliders = malloc(sizeof(Collider *));
        if (currColliderManager->allColliders == NULL){
            reportErreur("colliderManager:addColliderToColliderManager(...): error malloc()");
        }
    }else{
        memcpy(temp, currColliderManager->allColliders, currColliderManager->allCollidersSize * sizeof(Collider *));
        //*** realloc() doesn't work... weird...
        //realloc(currColliderManager->allColliders, (currColliderManager->allCollidersSize + 1) * sizeof(Collider *));
        free(currColliderManager->allColliders);
        currColliderManager->allColliders = malloc((currColliderManager->allCollidersSize + 1) * sizeof(Collider *));
        if (currColliderManager->allColliders == NULL){
            reportErreur("colliderManager:addColliderToColliderManager(...): error realloc()");
        }
        memcpy(currColliderManager->allColliders, temp, currColliderManager->allCollidersSize * sizeof(Collider *));
    }
    // Add the new Collider:
    currColliderManager->allCollidersSize += 1;
    currColliderManager->allColliders[currColliderManager->allCollidersSize - 1] = currColl;

}//------------------------------------------------------------------------------------------------------------------------

Collider * createCollider(ColliderManager *currColliderManager, int width, int height, int posX, int posY, int isEnabled, int ownerTag){
// Create a new collider and add it to a colliderManager
    Collider *res = initCollider(getNextAvailableColliderId(currColliderManager), width, height, posX, posY, isEnabled, ownerTag);
    addColliderToColliderManager(currColliderManager, res);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider * colliderDeepCopyByColliderManager(ColliderManager *currColliderManager, Collider *coll){
// Make a deep copy (=create a different reference) from a Collider and add it to a colliderManager
    Collider *res = NULL;
    if (coll != NULL){
        res = malloc(sizeof(Collider));
        if (res == NULL){
            reportErreur("Error malloc colliderDeepCopy()");
        }
        memcpy(res, coll, sizeof(Collider));
        res->id = getNextAvailableColliderId(currColliderManager);
        addColliderToColliderManager(currColliderManager, res);
    }
    //printf("CollMgr: added: %d \n", res->id);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider ** colliderArrayDeepCopyByColliderManager(ColliderManager *currColliderManager, Collider **coll, int currSize){
// Make a deep copy (=create a different reference) from a Collider array and add it to a colliderManager
    Collider **res = NULL;
    int i;
    if (coll != NULL && currSize != 0){
        res = malloc(currSize * sizeof(Collider *));
        if (res == NULL){
            reportErreur("Error malloc colliderArrayDeepCopy()");
        }
        for(i=0; i < currSize; i++){
            res[i] = colliderDeepCopyByColliderManager(currColliderManager, coll[i]);
        }
    }
    return res;
}//------------------------------------------------------------------------------------------------------------------------

int removeCollider(ColliderManager *currColliderManager, int idCollider){
// Destroy a collider
    int i;
    int wasFounded = 0;
    for(i = 0; i < currColliderManager->allCollidersSize; i++){
        if(currColliderManager->allColliders[i]->id == idCollider){
            destroyCollider(currColliderManager->allColliders[i]);
            if (wasFounded){
                reportErreur("destroyCollider(): Error multiple occurence of this ID");
            }
            wasFounded = 1;
        }
        // Put the collider to delete at the end
        if (wasFounded && i < currColliderManager->allCollidersSize - 1){
            currColliderManager->allColliders[i] = currColliderManager->allColliders[i + 1];
            i -= 1;
        }
    }

    if( realloc(currColliderManager->allColliders, (currColliderManager->allCollidersSize - 1) * sizeof(Collider *)) == NULL){
        reportErreur("colliderManager:destroyCollider(...): error realloc()");
    }
    currColliderManager->allCollidersSize -= 1;
    return wasFounded;
}//------------------------------------------------------------------------------------------------------------------------

Collider * getColliderById(ColliderManager *collMgr, int id){
// Get a Collider by it's Id
    int i;
    for(i = 0; i < collMgr->allCollidersSize; i++){
        if( collMgr->allColliders[i]->id == id){
            return collMgr->allColliders[i];
        }
    }
    reportErreur("colliderManager.c:getColliderById(): cannot find collider with given Id");
    return NULL;
}//------------------------------------------------------------------------------------------------------------------------

void createCollision(ColliderManager *currCollMgr, int coll1Id, int coll2Id){
// Create a new Collision and add it to the manager array
    int i,j, isAvailable, availableId, isCollisionAlreadyExists;
    Collision *temp [currCollMgr->allCollisionsSize];

    // Test if this Collision already exists
    isCollisionAlreadyExists = 0;
    for(i = 0; i < currCollMgr->allCollisionsSize; i++){
        if ((currCollMgr->allCollisions[i]->coll1Id == coll1Id && currCollMgr->allCollisions[i]->coll2Id == coll2Id)
        ||(currCollMgr->allCollisions[i]->coll1Id == coll2Id && currCollMgr->allCollisions[i]->coll2Id == coll1Id)){
            isCollisionAlreadyExists = 1;
            break;
        }
    }

    // Find a available Id
    for(i = 0; !isCollisionAlreadyExists && i < currCollMgr->allCollisionsSize - 1 ; i++){
        isAvailable = 1;
        for(j = 0; j < currCollMgr->allCollisionsSize - 1; j++){
            if (currCollMgr->allCollisions[j]->id == i){
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
        availableId = currCollMgr->allCollisionsSize - 1;
    }

    // Add 1 field to the Collisions array and fill it
    if (!isCollisionAlreadyExists){
        if( currCollMgr->allCollisions == NULL || currCollMgr->allCollisionsSize == 0){
            currCollMgr->allCollisions = malloc(sizeof(Collision*));
            if (currCollMgr->allCollisions == NULL){reportErreur("colliderManager:createCollision(...): error malloc()1");}
        }else{
            memcpy(temp, currCollMgr->allCollisions, currCollMgr->allCollisionsSize * sizeof(Collision*));
            //*** realloc doesn't work...
                //realloc(currCollMgr->allCollisions, (currCollMgr->allCollisionsSize + 1) * sizeof(Collision*));
            free(currCollMgr->allCollisions);
            currCollMgr->allCollisions = malloc((currCollMgr->allCollisionsSize + 1) * sizeof(Collision*));
            if (currCollMgr->allCollisions == NULL){reportErreur("colliderManager:createCollision(...): error malloc()2");}
            memcpy(currCollMgr->allCollisions, temp, currCollMgr->allCollisionsSize * sizeof(Collision*));
        }
        currCollMgr->allCollisionsSize += 1;
        // Create the new Collision
        currCollMgr->allCollisions[currCollMgr->allCollisionsSize - 1] = initCollision(availableId, coll1Id, coll2Id);
    }
}//------------------------------------------------------------------------------------------------------------------------

int getNextAvailableColliderId(ColliderManager *currColliderManager){
// Get the next available id for a collider
    int i, j, isAvailable;
    for(i = 0; i < currColliderManager->allCollidersSize; i++){
        isAvailable = 1;
        for(j = 0; j < currColliderManager->allCollidersSize; j++){
            if(currColliderManager->allColliders[j]->id == i){
                isAvailable = 0;
                break;
            }
        }
        if (isAvailable){
            return i;
        }
    }
    return currColliderManager->allCollidersSize;
}//------------------------------------------------------------------------------------------------------------------------

void updateCollisions(ColliderManager *currColliderManager, int leftLimit, int rightLimit, int bottomLimit, int topLimit){
// Update all the collisions between each collider
    int i, j;
    int temp;
    // Clearing previous Collisions
    if (currColliderManager->allCollisions != NULL){
        for(i = 0; i < currColliderManager->allCollisionsSize; i++){
            destroyCollision(currColliderManager->allCollisions[i]);
        }
        free(currColliderManager->allCollisions);
        currColliderManager->allCollisions = NULL;
        currColliderManager->allCollisionsSize = 0;
    }


    for(i = 0; i < currColliderManager->allCollidersSize; i++){
        //*** Temporary
        if (currColliderManager->allColliders[i]->isEnabled){
            printf("collider enabled (x, y, w, h): %d,%d,%d,%d \n",
                   currColliderManager->allColliders[i]->posX, currColliderManager->allColliders[i]->posY
                   , currColliderManager->allColliders[i]->width, currColliderManager->allColliders[i]->height);
        }
        // Only takes collider enabled and inside the screen (leftLimit, rightLimit, bottomLimit, topLimit)
        if (currColliderManager->allColliders[i]->isEnabled
        && currColliderManager->allColliders[i]->posX < rightLimit
        && currColliderManager->allColliders[i]->posX + currColliderManager->allColliders[i]->width > leftLimit
        && currColliderManager->allColliders[i]->posY < topLimit
        && currColliderManager->allColliders[i]->posY + currColliderManager->allColliders[i]->height > bottomLimit){
            for(j = 0; j < currColliderManager->allCollidersSize; j++){
                if ( i != j && currColliderManager->allColliders[j]->isEnabled){

                    if(currColliderManager->allColliders[i]->posX < currColliderManager->allColliders[j]->posX + currColliderManager->allColliders[j]->width
                    && currColliderManager->allColliders[i]->posX + currColliderManager->allColliders[i]->width > currColliderManager->allColliders[j]->posX){
                        if(currColliderManager->allColliders[i]->posY < currColliderManager->allColliders[j]->posY + currColliderManager->allColliders[j]->height
                        && currColliderManager->allColliders[i]->posY + currColliderManager->allColliders[i]->height > currColliderManager->allColliders[j]->posY){
                            createCollision(currColliderManager, currColliderManager->allColliders[i]->id, currColliderManager->allColliders[j]->id);

                        }
                    }
                }
            }// for end
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void getColliderTouching(ColliderManager *collMgr,int colliderId, Collider ***res, int *resSize){
// Find Colliders which collide with given id Collider
    int i;

    *res = malloc(collMgr->allCollisionsSize * sizeof(Collider *));
    *resSize = 0;
    if (res == NULL){
        reportErreur("colliderManager.c:getColliderTouching(): error malloc()");
    }

    for(i = 0; i < collMgr->allCollisionsSize; i++){
        if(collMgr->allCollisions[i]->coll1Id == colliderId){
            (*res)[*resSize] = getColliderById(collMgr, collMgr->allCollisions[i]->coll2Id);
            *resSize += 1;
        }else if(collMgr->allCollisions[i]->coll2Id == colliderId){
            (*res)[*resSize] = getColliderById(collMgr, collMgr->allCollisions[i]->coll1Id);
            *resSize += 1;
        }
    }
    // Reduce res length
    if(*resSize < collMgr->allCollisionsSize){
        realloc(*res, *resSize * sizeof(Collider *));
    }
}//------------------------------------------------------------------------------------------------------------------------

void destroyColliderManager(ColliderManager *currColliderManager){
// Free colliderManager memory
    int i;
    for(i = 0; i < currColliderManager->allCollidersSize; i++){
        destroyCollider(currColliderManager->allColliders[i]);
    }
    free(currColliderManager->allColliders);

    for(i = 0; i < currColliderManager->allCollisionsSize; i++){
        destroyCollision(currColliderManager->allCollisions[i]);
    }
    free(currColliderManager->allCollisions);
}//------------------------------------------------------------------------------------------------------------------------

