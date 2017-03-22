#include "heroInstance.h"


HeroInstance * initHeroInstance(){
// Create a new HeroInstance
    HeroInstance *res = malloc(sizeof(HeroInstance));
    if(res == NULL){
        reportErreur("initHeroInstance(): error malloc()");
    }

    res->idHeros = 0;
    res->currState = 0;// Is tux big(1) or small(0)
    res->currAction = 0;
    res->currSprite = 0;
    res->isDead = 0;
    res->lifesLeft = 2;
    res->nbCoins = 0;
    res->lastMovementTime = 0;// Last time when tux move
    res->isTouchingGround = 0;
    res->jumpKeyPressed = 0;// (0 or 1)When player press the jump key
    res->jumpStartTime = 0;// Time at jump start
    res->jumpDuration = 0;// Time between jump start and jump max height
    res->posX = 150;
    res->posY = 300;
    res->lastPosX = 150;
    res->lastPosY = 300;
    res->movementProgressX = 0;
    res->movementProgressY = 0;
    res->currentTime = 0;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void refreshHeroInstance(HeroInstance * p_heroInstance, Heros *p_heros, int loopTime){
// Refresh hero's sprites
    int movementY;
    p_heroInstance->currentTime += loopTime;
    // Changing sprite
    if(p_heroInstance->currentTime >= p_heros->spriteDuration[p_heroInstance->currState][p_heroInstance->currAction]){
        p_heroInstance->currentTime = 0;
        p_heroInstance->currSprite += 1;
        p_heroInstance->currSprite = p_heroInstance->currSprite % p_heros->spriteSize[p_heroInstance->currState][p_heroInstance->currAction];
    }

    // Gravity
    p_heroInstance->movementProgressY -= loopTime / 1000.0 * GRAVITY_SPEED;
    movementY = (int) p_heroInstance->movementProgressY;
    if ( movementY >= 1 || movementY <= -1){
        p_heroInstance->posY += movementY;
        p_heroInstance->movementProgressY -= movementY;
    }

}//------------------------------------------------------------------------------------------------------------------------

/*void killHeroInstance(Hero s * currHeros){
// Kill a Heros
    currHeros->isDead = 1;
}//------------------------------------------------------------------------------------------------------------------------
*/
void destroyHeroInstanceColliders(HeroInstance *p_heroInstance, Heros *p_heros){
// Free HeroInstance's colliders
    int i, j;
    for(i = 0; i < p_heros->stateSize; i++){
        for(j = 0; j < p_heros->actionSize[i]; j++){
            destroyCollider(p_heroInstance->herosColl[i][j]);
        }
        free(p_heroInstance->herosColl[i]);
    }
    free(p_heroInstance->herosColl);

}//------------------------------------------------------------------------------------------------------------------------

void destroyHeroInstance(HeroInstance *p_heroInstance){
// Free HeroInstance memory
    free(p_heroInstance);
}//------------------------------------------------------------------------------------------------------------------------
