#include "herosManager.h"





HerosManager *initHerosManager(char p_path[FILE_PATH_SIZE_MAX]){
// Init a new HerosManager
    int i;
    HerosManager *res = malloc(sizeof(HerosManager));
    strncpy(res->path, p_path, FILE_PATH_SIZE_MAX);

    if(res == NULL){
        reportErreur("initHerosManager():error malloc()");
    }
    res->heros = initHeros(res->path);
    initHerosInstanceByManager(res);

    return res;
}//------------------------------------------------------------------------------------------------------------------------

void initHerosInstanceByManager(HerosManager *p_herosMgr){
// Initialize a new herosInstance

    //if(herosMgr->heroInstance != NULL){destroyHeroInstance(herosMgr->heroInstance);}
    p_herosMgr->heroInstance = initHeroInstance();
}//------------------------------------------------------------------------------------------------------------------------

void updateHeroBehaviourAfterCollisionDetection(HeroInstance *p_herosInstance, Heros * p_heros, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, LevelManager *p_levMgr, FireBullet *p_fireBullet){
// Update the heros behaviour AFTER having some collisions
    Collider **contactPoints;
    int contactPointsSize;
    int i;
    int isAboveSomething = 0;
    int isTouchingEnemy = 0;

    // Collision detection: get all the colliders which are colliding with the heros
    getColliderTouching(p_collMgr, p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->id
                        , &contactPoints, &contactPointsSize);
    for (i = 0; i < contactPointsSize; i++){
        switch(contactPoints[i]->ownerTag){
            case tag_bonus_coin:
                p_herosInstance->nbCoins += 1;
                if(p_herosInstance->nbCoins >= 100){
                    p_herosInstance->nbCoins -= 100;
                    p_herosInstance->lifesLeft += 1;
                }
                break;
            case tag_bonus_egg:
                if(p_herosInstance->currState == 0){// To avoid a fire tux (state 2) to become a big tux (state 1)
                    if(p_herosInstance->lastDirection == 'r'){
                        changeHerosState(p_herosInstance, 1, 0, 0);
                    }else{
                        changeHerosState(p_herosInstance, 1, 1, 0);
                    }
                }
                break;
            case tag_bonus_flower:
                if(p_herosInstance->lastDirection == 'r'){
                    changeHerosState(p_herosInstance, 2, 0, 0);
                }else{
                    changeHerosState(p_herosInstance, 2, 1, 0);
                }
                break;
            case tag_explosion:
                isTouchingEnemy = 1;
                break;
            default:
                // Testing if the hero is above the colliding object
                if( p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosY >= contactPoints[i]->posY + contactPoints[i]->height){
                    p_herosInstance->posY = contactPoints[i]->posY + contactPoints[i]->height;
                    isAboveSomething = 1;
                    // Bouncing on an enemy
                    if(contactPoints[i]->ownerTag == tag_enemy_fluffy || contactPoints[i]->ownerTag == tag_enemy_bomb || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                        p_herosInstance->jumpStartTime = p_currentTime;
                        p_herosInstance->jumpDuration = p_heros->jumpDuration / 2;
                    }
                // Testing if the hero is under the colliding object
                }else if(p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->lastPosY + p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->height <= contactPoints[i]->posY){
                    p_herosInstance->posY = contactPoints[i]->posY - p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->height;
                    // Stop the jump
                    p_herosInstance->jumpStartTime = -1;
                    p_herosInstance->jumpDuration = -1;
                    // Touching an enemy
                    if(contactPoints[i]->ownerTag == tag_enemy_fluffy || contactPoints[i]->ownerTag == tag_enemy_bomb || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                        isTouchingEnemy = 1;
                    }
                }else{// Lateral collision
                    // If left touching
                    if (p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX < contactPoints[i]->posX){
                        p_herosInstance->posX = contactPoints[i]->posX - p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->width;
                    }else{ // If right touching
                        p_herosInstance->posX = contactPoints[i]->posX + contactPoints[i]->width;
                    }
                    // Touching an enemy
                    if(contactPoints[i]->ownerTag == tag_enemy_fluffy || contactPoints[i]->ownerTag == tag_enemy_bomb || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                        isTouchingEnemy = 1;
                    }
                }
        }// End of switch
    }

    // Tux becomes smaller when he touches an enemy
    if(isTouchingEnemy && p_herosInstance->godModeDuration <= 0){
        printf("the hero died");
        if(p_herosInstance->currState == 1 || p_herosInstance->currState == 2){
            p_herosInstance->godModeDuration = 1000;
            if(p_herosInstance->lastDirection == 'r'){
                changeHerosState(p_herosInstance, p_herosInstance->currState - 1, 0, 0);
            }else{
                changeHerosState(p_herosInstance, p_herosInstance->currState - 1, 1, 0);
            }
        }
    }

    // Firing
    if(p_herosInstance->fireKeyPressed && p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->ownerState == state_tux_fire
       && p_herosInstance->timeBeforeNextShot <= 0 && p_herosInstance->hasReleaseFireKey){
        p_herosInstance->hasReleaseFireKey = 0;
        if(p_herosInstance->lastDirection == 'r'){
            addBulletInstanceFromLevelMgr(p_collMgr, p_levMgr, p_herosInstance->posX + p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->width + 1
                , p_herosInstance->posY + p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->height / 2, p_fireBullet, 1);
            changeHerosAction(p_herosInstance, 7, 0);
        }else{
            addBulletInstanceFromLevelMgr(p_collMgr, p_levMgr, p_herosInstance->posX - 1 - p_fireBullet->refColl->width, p_herosInstance->posY + p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->height / 2, p_fireBullet, -1);
            changeHerosAction(p_herosInstance, 8, 0);
        }

        p_herosInstance->timeBeforeNextShot = TIME_BETWEEN_SHOTS;
    }

    // Is the heros is on the ground (or on an enemy)
    if(isAboveSomething){
        p_herosInstance->isTouchingGround = 1;
        // Start a new jump
        if(p_herosInstance->jumpKeyPressed && p_herosInstance->hasReleaseJumpKey){
            p_herosInstance->hasReleaseJumpKey = 0;
            if(p_herosInstance->lastDirection == 'r'){
                changeHerosAction(p_herosInstance, 5, 0);
            }else{
                changeHerosAction(p_herosInstance, 6, 0);
            }
            p_herosInstance->jumpStartTime = p_currentTime;
            p_herosInstance->jumpDuration = 0;
        }else{ // When the heros touch the ground (or an ennemy, bonus,...) after falling
            if(p_herosInstance->currAction == 5){
                changeHerosAction(p_herosInstance, 0, 0);
            }
            p_herosInstance->jumpDuration = -1;
        }
    }else{
        p_herosInstance->isTouchingGround = 0;
        p_herosInstance->jumpStartTime = -1;
    }

    // Update Collider position
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posX = p_herosInstance->posX;
    p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->posY = p_herosInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------

void jumpHeros(Heros *p_heros){

}//------------------------------------------------------------------------------------------------------------------------

void moveHeros(Heros *p_heros, int p_direction, int p_loopTime){

}//------------------------------------------------------------------------------------------------------------------------

void displayHeros(HerosManager *p_herosMgr, SDL_Window *p_window, int p_displayedPosX, int p_displayedPosY){
// Display the heros
    SDL_Rect objectPos = {0, 0, 0, 0};
    objectPos.x = p_displayedPosX;;
    objectPos.y = SDL_GetWindowSurface(p_window)->h - p_displayedPosY
        - p_herosMgr->heroInstance->herosColl[p_herosMgr->heroInstance->currState][p_herosMgr->heroInstance->currAction]->height;

    SDL_BlitSurface(p_herosMgr->heros->sprites[p_herosMgr->heroInstance->currState][p_herosMgr->heroInstance->currAction][p_herosMgr->heroInstance->currSprite],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
}//------------------------------------------------------------------------------------------------------------------------

void testHerosCollisions(Heros *p_heros){

}//------------------------------------------------------------------------------------------------------------------------

void herosGatherBonus(Heros* p_heros, BonusInstance *p_bonusInstance){

}//------------------------------------------------------------------------------------------------------------------------

void destroyHerosManager(HerosManager *p_herosManager){
// Free HerosManager memory
    destroyHeros(p_herosManager->heros);
    if(p_herosManager->heroInstance != NULL){
        destroyHeroInstance(p_herosManager->heroInstance);
    }
    free(p_herosManager);
}//------------------------------------------------------------------------------------------------------------------------

