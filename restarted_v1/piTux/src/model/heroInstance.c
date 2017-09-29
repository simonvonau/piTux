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
    res->lifesLeft = 0;
    res->nbCoins = 0;
    res->lastMovementTime = 0;// Last time when tux move
    res->isTouchingGround = 0;
    res->jumpKeyPressed = 0;// (0 or 1)When player press the jump key
    res->fireKeyPressed = 0;
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
    res->timeBeforeNextShot = TIME_BETWEEN_SHOTS;
    res->hasReleaseFireKey = 1;
    res->hasReleaseJumpKey = 1;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void changeHerosState(HeroInstance * p_herosInstance,int p_newState, int p_newAction, int p_newSprite){
// Change herosInstance state (big/small/fire)

    if(p_herosInstance->currState == p_newState){return;}
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->isEnabled = 0;

    // Put the old coordinate in the new enabled collider
    p_herosInstance->herosColl[p_newState][p_newAction]->posX = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX;
    p_herosInstance->herosColl[p_newState][p_newAction]->posY = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posY;
    p_herosInstance->herosColl[p_newState][p_newAction]->lastPosX = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosX;
    p_herosInstance->herosColl[p_newState][p_newAction]->lastPosY = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosY;

    p_herosInstance->currState = p_newState;
    p_herosInstance->currAction = p_newAction;
    p_herosInstance->currSprite = p_newSprite;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->isEnabled = 1;
}//------------------------------------------------------------------------------------------------------------------------

void changeHerosAction(HeroInstance * p_herosInstance, int p_newAction, int p_newSprite){
// Change the herosInstance current action

    if(p_herosInstance->currAction == p_newAction){return;}
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->isEnabled = 0;

    // Put the old coordinate in the new enabled collider
    p_herosInstance->herosColl[p_herosInstance->currState][p_newAction]->posX = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX;
    p_herosInstance->herosColl[p_herosInstance->currState][p_newAction]->posY = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posY;
    p_herosInstance->herosColl[p_herosInstance->currState][p_newAction]->lastPosX = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosX;
    p_herosInstance->herosColl[p_herosInstance->currState][p_newAction]->lastPosY = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosY;

    p_herosInstance->currAction = p_newAction;
    p_herosInstance->currSprite = p_newSprite;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->isEnabled = 1;
}//------------------------------------------------------------------------------------------------------------------------

void refreshHerosInstance(HeroInstance * p_herosInstance, Heros *p_heros, int p_currentTime, int p_loopTime){
// Refresh hero's position and sprite
    int movementX, movementY;
    float coeff = 1.5;
    p_herosInstance->currentTime += p_loopTime;
    p_herosInstance->timeBeforeNextShot -= p_loopTime;
    if(p_herosInstance->godModeDuration > 0){
        p_herosInstance->godModeDuration -= p_loopTime;
    }

    applyGravity(p_herosInstance, p_loopTime);

    if(!p_herosInstance->isDead){
        jump(p_herosInstance, p_heros, p_loopTime);
        land(p_herosInstance);

        movingRight(p_herosInstance, p_heros, p_loopTime);
        movingLeft(p_herosInstance, p_heros, p_loopTime);

        // Go to idle mode
        if(!p_herosInstance->leftKeyPressed  && !p_herosInstance->rightKeyPressed && !p_herosInstance->jumpKeyPressed
            && !p_herosInstance->fireKeyPressed && p_herosInstance->isTouchingGround){
            if(p_herosInstance->lastDirection == 'r'){
                changeHerosAction(p_herosInstance, 0, 0);
            }else{
                changeHerosAction(p_herosInstance, 1, 0);
            }

        }
    }else{
        if(p_herosInstance->jumpDuration >= p_heros->jumpDuration * 0.75){
            coeff = 1.0 * p_herosInstance->jumpDuration / p_heros->jumpDuration;
        }
        if(p_herosInstance->jumpDuration <= p_heros->jumpDuration){
            p_herosInstance->jumpDuration += p_loopTime;
            p_herosInstance->movementProgressY += p_loopTime / 1000.0 * p_heros->jumpSpeed * coeff;
            movementY = (int) p_herosInstance->movementProgressY;
            if(movementY >= 1 || movementY <= -1){
                p_herosInstance->posY += movementY;
                p_herosInstance->movementProgressY -= movementY;
            }
        }
    }

    updateCurrentSprite(p_herosInstance, p_heros);

    // Update Collider position
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosX = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosY = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posY;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX = p_herosInstance->posX;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posY = p_herosInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------

void applyGravity(HeroInstance *p_herosInstance, int p_loopTime){
// Apply gravity force to tux
    int movementY;

    p_herosInstance->movementProgressY -= p_loopTime / 1000.0 * GRAVITY_SPEED;
    movementY = (int) p_herosInstance->movementProgressY;
    if ( movementY >= 1 || movementY <= -1){
        p_herosInstance->posY += movementY;
        p_herosInstance->movementProgressY -= movementY;
    }
}//------------------------------------------------------------------------------------------------------------------------

void movingRight(HeroInstance *p_herosInstance, Heros *p_heros, int p_loopTime){
// Moving right
    int movementX;

    if(p_herosInstance->rightKeyPressed){
        p_herosInstance->lastDirection = 'r';
        if(p_herosInstance->isTouchingGround && p_herosInstance->currAction != 3){
            changeHerosAction(p_herosInstance, 3, 0);
        }
        p_herosInstance->movementProgressX += p_loopTime / 1000.0 * p_heros->speed;
        movementX = (int) p_herosInstance->movementProgressX;
        if(movementX >= 1 || movementX <= -1){
            p_herosInstance->posX += movementX;
            p_herosInstance->movementProgressX -= movementX;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void movingLeft(HeroInstance *p_herosInstance, Heros *p_heros, int p_loopTime){
// Moving left
    int movementX;

    if(p_herosInstance->leftKeyPressed){
        p_herosInstance->lastDirection = 'l';
        if(p_herosInstance->isTouchingGround && p_herosInstance->currAction != 4){
            changeHerosAction(p_herosInstance, 4, 0);
        }
        p_herosInstance->movementProgressX -= p_loopTime / 1000.0 * p_heros->speed;
        movementX = (int) p_herosInstance->movementProgressX;
        if(movementX >= 1 || movementX <= -1){
            p_herosInstance->posX += movementX;
            p_herosInstance->movementProgressX -= movementX;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void jump(HeroInstance *p_herosInstance, Heros *p_heros, int p_loopTime){
    // When the heros jump (just the step when he go up)
    int movementY;
    float coeff = 1;

    // Increase the jump high when the key jump is still pressed
    /*if(p_herosInstance->jumpKeyPressed){
        coeff = 1.5;
    }*/

    if(p_herosInstance->jumpDuration >= 0 && p_herosInstance->jumpDuration < p_heros->jumpDuration * coeff){
        // Set the right or left sprite
        if(p_herosInstance->rightKeyPressed){
            p_herosInstance->lastDirection = 'r';
            changeHerosAction(p_herosInstance, 5, 0);
        }else if(p_herosInstance->leftKeyPressed){
            p_herosInstance->lastDirection = 'l';
            changeHerosAction(p_herosInstance, 6, 0);
        }

        p_herosInstance->jumpDuration += p_loopTime;
        p_herosInstance->movementProgressY += p_loopTime / 1000.0 * p_heros->jumpSpeed;
        movementY = (int) p_herosInstance->movementProgressY;
        if(movementY >= 1 || movementY <= -1){
            p_herosInstance->posY += movementY;
            p_herosInstance->movementProgressY -= movementY;
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void land(HeroInstance *p_herosInstance){
// When tux lands after a jump or a fall
    if(!p_herosInstance->isTouchingGround){
        if(p_herosInstance->rightKeyPressed){
            p_herosInstance->lastDirection = 'r';
            changeHerosAction(p_herosInstance, 5, 0);
        }else if(p_herosInstance->leftKeyPressed){
            p_herosInstance->lastDirection = 'l';
            changeHerosAction(p_herosInstance, 6, 0);
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

void gatherCoin(HeroInstance *p_heroInstance){
// When tux gather a coin
    p_heroInstance->nbCoins += 1;
    if(p_heroInstance->nbCoins >= 100){
        p_heroInstance->lifesLeft += 1;
        p_heroInstance->nbCoins -= 100;
    }

}//------------------------------------------------------------------------------------------------------------------------

void heroInstanceDeath(HeroInstance *p_herosInstance, Heros *p_heros, int p_currentTime){
// When tux dies
    int i;
    p_herosInstance->lifesLeft -= 1;
    p_herosInstance->isDead = 1;
    p_herosInstance->jumpDuration = 0;
    p_herosInstance->jumpStartTime = p_currentTime;
    changeHerosAction(p_herosInstance, 2, 0);
    for(i = 0; i < p_heros->actionSize[p_herosInstance->currState]; i++){
        p_herosInstance->herosColl[p_herosInstance->currState][i]->isEnabled = 0;
    }

}//------------------------------------------------------------------------------------------------------------------------


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
