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
    res->currentTime = 0;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

/*void refreshHeroInstance(Heros * p_heros, int loopTime){
// Refresh hero's sprites
    p_heros->currentTime += loopTime;
    // Changing sprite
    if(p_heros->currentTime >= p_heros->spriteDuration[p_heros->currState][p_heros->currAction]){
        p_heros->currentTime = 0;
        p_heros->currSprite += 1;
        p_heros->currSprite = p_heros->currSprite % p_heros->spriteSize[p_heros->currState][p_heros->currAction];
    }

}//------------------------------------------------------------------------------------------------------------------------
*/
/*void killHeroInstance(Hero s * currHeros){
// Kill a Heros
    currHeros->isDead = 1;
}//------------------------------------------------------------------------------------------------------------------------
*/

void destroyHeroInstance(HeroInstance *p_heroInstance){
// Free HeroInstance memory
    free(p_heroInstance);
}//------------------------------------------------------------------------------------------------------------------------
