#include "collision.h"

Collision * initCollision(int id, int currColl1Id, int currColl2Id){
    Collision *res = malloc(sizeof(Collision));
    if (res == NULL){
        reportErreur("Error malloc initCollision()");
    }

    res->id = id;
    res->coll1Id = currColl1Id;
    res->coll2Id = currColl2Id;

    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyCollision(Collision *currCollision){
    free(currCollision);
}//------------------------------------------------------------------------------------------------------------------------


