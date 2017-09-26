#include "collider.h"

Collider *initCollider(int p_id, int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, enum ObjectTag p_ownerTag, enum ObjectState p_ownerState){
    Collider *res = malloc(sizeof(Collider));
    if (res == NULL){
        reportErreur("Error malloc initCollider()");
    }

    res->id = p_id;
    res->width = p_width;
    res->height = p_height;
    res->posX = p_posX;
    res->posY = p_posY;
    res->lastPosX = p_posX;
    res->lastPosY = p_posY;
    res->isEnabled = p_isEnabled;
    res->ownerTag = p_ownerTag;
    res->ownerState = p_ownerState;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

Collider *initNonRegisteredCollider(int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, enum ObjectTag p_ownerTag, enum ObjectState p_ownerState){
// Initialize a collider which is not registered in the collider manager
    return initCollider(-1, p_width, p_height, p_posX, p_posY, p_isEnabled, p_ownerTag, p_ownerState);

}//------------------------------------------------------------------------------------------------------------------------

void destroyCollider(Collider *p_collider){
    free(p_collider);
}//------------------------------------------------------------------------------------------------------------------------


