#include "herosManager.h"





HerosManager *initHerosManager(char path[FILE_PATH_SIZE_MAX]){
// Init a new HerosManager
    int i;
    HerosManager *res = malloc(sizeof(HerosManager));
    strncpy(res->path, path, FILE_PATH_SIZE_MAX);

    if(res == NULL){
        reportErreur("initHerosManager():error malloc()");
    }
    res->heros = initHeros(res->path);
    initHerosInstanceByManager(res);

    return res;
}//------------------------------------------------------------------------------------------------------------------------

void initHerosInstanceByManager(HerosManager *herosMgr){
// Initialize a new herosInstance

    //if(herosMgr->heroInstance != NULL){destroyHeroInstance(herosMgr->heroInstance);}
    herosMgr->heroInstance = initHeroInstance();
}//------------------------------------------------------------------------------------------------------------------------

void updateHeroBehaviourAfterCollisionDetection(HeroInstance *currHeroInstance, Heros * p_heros, ColliderManager *collMgr, int currentTime, int loopTime){
// Update the heros behaviour AFTER having some collisions
    Collider **contactPoints;
    int contactPointsSize;
    int i;
    int isAboveSomething = 0;
    int isTouchingEnemy = 0;

    // Collision detection: get all the colliders which are colliding with the heros
    getColliderTouching(collMgr, currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->id
                        , &contactPoints, &contactPointsSize);
    for (i = 0; i < contactPointsSize; i++){
        switch(contactPoints[i]->ownerTag){
            case TAG_BONUS_COIN:
                currHeroInstance->nbCoins += 1;
                if(currHeroInstance->nbCoins >= 100){
                    currHeroInstance->nbCoins -= 100;
                    currHeroInstance->lifesLeft += 1;
                }
                break;
            case TAG_BONUS_EGG:
                if(currHeroInstance->currState == 0){// To avoid a fire tux (state 2) to become a big tux (state 1)
                    if(currHeroInstance->lastDirection == 'r'){
                        changeHerosState(currHeroInstance, 1, 0, 0);
                    }else{
                        changeHerosState(currHeroInstance, 1, 1, 0);
                    }
                }
                break;
            case TAG_BONUS_FIREFLOWER:
                if(currHeroInstance->lastDirection == 'r'){
                    changeHerosState(currHeroInstance, 2, 0, 0);
                }else{
                    changeHerosState(currHeroInstance, 2, 1, 0);
                }
                break;
            case TAG_EXPLOSION:
                isTouchingEnemy = 1;
                break;
            default:
                // Testing if the hero is above the colliding object
                if( currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->lastPosY >= contactPoints[i]->posY + contactPoints[i]->height){
                    currHeroInstance->posY = contactPoints[i]->posY + contactPoints[i]->height;
                    isAboveSomething = 1;
                    // Bouncing on an enemy
                    if(contactPoints[i]->ownerTag == TAG_ENEMY_MRFLUFFY || contactPoints[i]->ownerTag == TAG_ENEMY_MRBOMB || contactPoints[i]->ownerTag == TAG_ENEMY_MRICEBLOCK){
                        currHeroInstance->jumpStartTime = currentTime;
                        currHeroInstance->jumpDuration = p_heros->jumpDuration / 2;
                    }
                // Testing if the hero is under the colliding object
                }else if(currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->lastPosY + currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->height <= contactPoints[i]->posY){
                    currHeroInstance->posY = contactPoints[i]->posY - currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->height;
                    // Stop the jump
                    currHeroInstance->jumpStartTime = -1;
                    currHeroInstance->jumpDuration = -1;
                    // Touching an enemy
                    if(contactPoints[i]->ownerTag == TAG_ENEMY_MRFLUFFY || contactPoints[i]->ownerTag == TAG_ENEMY_MRBOMB || contactPoints[i]->ownerTag == TAG_ENEMY_MRICEBLOCK){
                        isTouchingEnemy = 1;
                    }
                }else{// Lateral collision
                    // If left touching
                    if (currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->posX < contactPoints[i]->posX){
                        currHeroInstance->posX = contactPoints[i]->posX - currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->width;
                    }else{ // If right touching
                        currHeroInstance->posX = contactPoints[i]->posX + contactPoints[i]->width;
                    }
                    // Touching an enemy
                    if(contactPoints[i]->ownerTag == TAG_ENEMY_MRFLUFFY || contactPoints[i]->ownerTag == TAG_ENEMY_MRBOMB || contactPoints[i]->ownerTag == TAG_ENEMY_MRICEBLOCK){
                        isTouchingEnemy = 1;
                    }
                }
        }// End of switch
    }

    // Tux becomes smaller when he touches an enemy
    if(isTouchingEnemy && currHeroInstance->godModeDuration <= 0){
        printf("the hero died");
        if(currHeroInstance->currState == 1 || currHeroInstance->currState == 2){
            currHeroInstance->godModeDuration = 1000;
            if(currHeroInstance->lastDirection == 'r'){
                changeHerosState(currHeroInstance, currHeroInstance->currState - 1, 0, 0);
            }else{
                changeHerosState(currHeroInstance, currHeroInstance->currState - 1, 1, 0);
            }
        }
    }

    // Is the heros is on the ground (or on an ennemy)
    if(isAboveSomething){
        currHeroInstance->isTouchingGround = 1;
        // Start a new jump
        if(currHeroInstance->jumpKeyPressed){
            if(currHeroInstance->lastDirection == 'r'){
                changeHeroAction(currHeroInstance, 5, 0);
            }else{
                changeHeroAction(currHeroInstance, 6, 0);
            }
            currHeroInstance->jumpStartTime = currentTime;
            currHeroInstance->jumpDuration = 0;
        }else{ // When the heros touch the ground (or an ennemy, bonus,...) after falling
            if(currHeroInstance->currAction == 5){
                changeHeroAction(currHeroInstance, 0, 0);
            }
        }
    }else{
        currHeroInstance->isTouchingGround = 0;
        currHeroInstance->jumpStartTime = -1;
    }

    // Update Collider position
    currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->posX = currHeroInstance->posX;
    currHeroInstance->herosColl[currHeroInstance->currState][currHeroInstance->currAction]->posY = currHeroInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------

void jumpHeros(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void moveHeros(Heros *currHeros, int direction, int timeLoop){

}//------------------------------------------------------------------------------------------------------------------------

void displayHeros(HerosManager *currHerosMgr, SDL_Window *p_window, int p_displayedPosX, int p_displayedPosY){
// Display the heros
    SDL_Rect objectPos = {0, 0, 0, 0};
    objectPos.x = p_displayedPosX;;
    objectPos.y = SDL_GetWindowSurface(p_window)->h - p_displayedPosY
        - currHerosMgr->heroInstance->herosColl[currHerosMgr->heroInstance->currState][currHerosMgr->heroInstance->currAction]->height;

    SDL_BlitSurface(currHerosMgr->heros->sprites[currHerosMgr->heroInstance->currState][currHerosMgr->heroInstance->currAction][currHerosMgr->heroInstance->currSprite],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
}//------------------------------------------------------------------------------------------------------------------------

void testHerosCollisions(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void herosGatherBonus(Heros* currHeros, BonusInstance *currBonusInstance){

}//------------------------------------------------------------------------------------------------------------------------

void destroyHerosManager(HerosManager *currHerosManager){
// Free HerosManager memory
    destroyHeros(currHerosManager->heros);
    if(currHerosManager->heroInstance != NULL){
        destroyHeroInstance(currHerosManager->heroInstance);
    }
    free(currHerosManager);
}//------------------------------------------------------------------------------------------------------------------------

