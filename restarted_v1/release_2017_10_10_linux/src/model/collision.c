#include "collision.h"

Collision * initCollision(int p_id, int p_coll1Id, int p_coll2Id){
    Collision *res = malloc(sizeof(Collision));
    if (res == NULL){
        reportErreur("Error malloc initCollision()");
    }

    res->id = p_id;
    res->coll1Id = p_coll1Id;
    res->coll2Id = p_coll2Id;

    return res;
}//------------------------------------------------------------------------------------------------------------------------

void destroyCollision(Collision *p_collision){
    free(p_collision);
}//------------------------------------------------------------------------------------------------------------------------


