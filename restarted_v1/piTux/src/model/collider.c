#include "collider.h"

Collider *initCollider(int id, int width, int height, int posX, int posY, int isEnabled, int ownerTag){
    Collider *res = malloc(sizeof(Collider));
    if (res == NULL){
        reportErreur("Error malloc initCollider()");
    }

    res->id = id;
    res->width = width;
    res->height = height;
    res->posX = posX;
    res->posY = posY;
    res->lastPosX = posX;
    res->lastPosY = posY;
    res->isEnabled = isEnabled;
    res->ownerTag = ownerTag;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider *initNonRegisteredCollider(int width, int height, int posX, int posY, int isEnabled, int ownerTag){
// Initialize a collider which is not registered in the collider manager
    return initCollider(-1, width, height, posX, posY, isEnabled, ownerTag);

}//------------------------------------------------------------------------------------------------------------------------

void destroyCollider(Collider *currCollider){
    free(currCollider);
}//------------------------------------------------------------------------------------------------------------------------


