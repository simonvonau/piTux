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
    res->leftKeyPressed = 0;
    res->rightKeyPressed = 0;
    res->jumpStartTime = -1;// Time at jump start
    res->jumpDuration = -1;// Time between jump start and jump max height
    res->lastDirection = 'r';
    res->posX = 150;
    res->posY = 500;
    res->lastPosX = 150;
    res->lastPosY = 300;
    res->movementProgressX = 0;
    res->movementProgressY = 0;
    res->currentTime = 0;
    res->godModeDuration = 0;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void changeHerosState(HeroInstance * p_heroInstance,int p_newState, int p_newAction, int p_newSprite){
    // Change herosInstance state (big/small/fire)

    if(p_heroInstance->currState == p_newState){return;}
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->isEnabled = 0;

    // Put the old coordinate in the new enabled collider
    p_heroInstance->herosColl[p_newState][p_newAction]->posX = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posX;
    p_heroInstance->herosColl[p_newState][p_newAction]->posY = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posY;
    p_heroInstance->herosColl[p_newState][p_newAction]->lastPosX = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosX;
    p_heroInstance->herosColl[p_newState][p_newAction]->lastPosY = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosY;

    p_heroInstance->currState = p_newState;
    p_heroInstance->currAction = p_newAction;
    p_heroInstance->currSprite = p_newSprite;
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->isEnabled = 1;
}//------------------------------------------------------------------------------------------------------------------------

void changeHeroAction(HeroInstance * p_heroInstance, int p_newAction, int p_newSprite){
    // Change the herosInstance current action

    if(p_heroInstance->currAction == p_newAction){return;}
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->isEnabled = 0;

    // Put the old coordinate in the new enabled collider
    p_heroInstance->herosColl[p_heroInstance->currState][p_newAction]->posX = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posX;
    p_heroInstance->herosColl[p_heroInstance->currState][p_newAction]->posY = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posY;
    p_heroInstance->herosColl[p_heroInstance->currState][p_newAction]->lastPosX = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosX;
    p_heroInstance->herosColl[p_heroInstance->currState][p_newAction]->lastPosY = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosY;

    p_heroInstance->currAction = p_newAction;
    p_heroInstance->currSprite = p_newSprite;
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->isEnabled = 1;
}//------------------------------------------------------------------------------------------------------------------------

void refreshHeroInstance(HeroInstance * p_heroInstance, Heros *p_heros, int currentTime, int loopTime){
// Refresh hero's position and sprite
    int movementX, movementY;
    p_heroInstance->currentTime += loopTime;
    if(p_heroInstance->godModeDuration > 0){
        p_heroInstance->godModeDuration -= loopTime;
    }

    applyGravity(p_heroInstance, loopTime);

    jump(p_heroInstance, p_heros, loopTime);
    land(p_heroInstance);

    movingRight(p_heroInstance, p_heros, loopTime);
    movingLeft(p_heroInstance, p_heros, loopTime);

    // Go to idle mode
    if(!p_heroInstance->leftKeyPressed  && !p_heroInstance->rightKeyPressed && !p_heroInstance->jumpKeyPressed && p_heroInstance->isTouchingGround){
        if(p_heroInstance->lastDirection == 'r'){
            changeHeroAction(p_heroInstance, 0, 0);
        }else{
            changeHeroAction(p_heroInstance, 1, 0);
        }

    }

    updateCurrentSprite(p_heroInstance, p_heros);

    // Update Collider position
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosX = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posX;
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->lastPosY = p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posY;
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posX = p_heroInstance->posX;
    p_heroInstance->herosColl[p_heroInstance->currState][p_heroInstance->currAction]->posY = p_heroInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------

void applyGravity(HeroInstance *p_heroInstance, int p_loopTime){
    // Apply gravity force to tux
    int movementY;

    p_heroInstance->movementProgressY -= p_loopTime / 1000.0 * GRAVITY_SPEED;
    movementY = (int) p_heroInstance->movementProgressY;
    if ( movementY >= 1 || movementY <= -1){
        p_heroInstance->posY += movementY;
        p_heroInstance->movementProgressY -= movementY;
    }
}//------------------------------------------------------------------------------------------------------------------------

void movingRight(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime){
    // Moving right
    int movementX;

    if(p_heroInstance->rightKeyPressed){
        p_heroInstance->lastDirection = 'r';
        if(p_heroInstance->isTouchingGround && p_heroInstance->currAction != 3){
            changeHeroAction(p_heroInstance, 3, 0);
        }
        p_heroInstance->movementProgressX += p_loopTime / 1000.0 * p_heros->speed;
        movementX = (int) p_heroInstance->movementProgressX;
        if(movementX >= 1 || movementX <= -1){
            p_heroInstance->posX += movementX;
            p_heroInstance->movementProgressX -= movementX;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void movingLeft(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime){
    // Moving left
    int movementX;

    if(p_heroInstance->leftKeyPressed){
        p_heroInstance->lastDirection = 'l';
        if(p_heroInstance->isTouchingGround && p_heroInstance->currAction != 4){
            changeHeroAction(p_heroInstance, 4, 0);
        }
        p_heroInstance->movementProgressX -= p_loopTime / 1000.0 * p_heros->speed;
        movementX = (int) p_heroInstance->movementProgressX;
        if(movementX >= 1 || movementX <= -1){
            p_heroInstance->posX += movementX;
            p_heroInstance->movementProgressX -= movementX;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void jump(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime){
    // When the heros jump (just the step when he go up)
    int movementY;

    if(p_heroInstance->jumpDuration >= 0 && p_heroInstance->jumpDuration < p_heros->jumpDuration){
        // Set the right or left sprite
        if(p_heroInstance->rightKeyPressed){
            p_heroInstance->lastDirection = 'r';
            changeHeroAction(p_heroInstance, 5, 0);
        }else if(p_heroInstance->leftKeyPressed){
            p_heroInstance->lastDirection = 'l';
            changeHeroAction(p_heroInstance, 6, 0);
        }
        p_heroInstance->jumpDuration += p_loopTime;
        p_heroInstance->movementProgressY += p_loopTime / 1000.0 * p_heros->jumpSpeed;
        movementY = (int) p_heroInstance->movementProgressY;
        if(movementY >= 1 || movementY <= -1){
            p_heroInstance->posY += movementY;
            p_heroInstance->movementProgressY -= movementY;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void land(HeroInstance *p_heroInstance){
    // When tux lands after a jump or a fall
    if(!p_heroInstance->isTouchingGround){
        if(p_heroInstance->rightKeyPressed){
            p_heroInstance->lastDirection = 'r';
            changeHeroAction(p_heroInstance, 5, 0);
        }else if(p_heroInstance->leftKeyPressed){
            p_heroInstance->lastDirection = 'l';
            changeHeroAction(p_heroInstance, 6, 0);
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void updateCurrentSprite(HeroInstance *p_heroInstance, Heros *p_heros){
    // Update the current sprite
    if(p_heroInstance->currentTime >= p_heros->spriteDuration[p_heroInstance->currState][p_heroInstance->currAction]){
        p_heroInstance->currentTime = 0;
        p_heroInstance->currSprite += 1;
        p_heroInstance->currSprite = p_heroInstance->currSprite % p_heros->spriteSize[p_heroInstance->currState][p_heroInstance->currAction];
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
    p_heroInstance = NULL;
}//------------------------------------------------------------------------------------------------------------------------
