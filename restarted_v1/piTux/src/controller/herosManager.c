#include "herosManager.h"





HerosManager *initHerosManager(char p_path[FILE_PATH_SIZE_MAX]){
// Init a new HerosManager
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
    int isAboveEnemy = 0;
    int isTouchingEnemy = 0;

    int verticalCollSize = 100;
    int verticalColl[verticalCollSize];
    int verticalCollId = 0;

    Collider *tux = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction];
    int leftWall = tux->posX - 2;
    int rightWall = tux->posX + tux->width + 2;
    int buffer;


    if(!p_herosInstance->isDead){

        // Collision detection: get all the colliders which are colliding with the heros
        getColliderTouching(p_collMgr, p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction]->id
                            , &contactPoints, &contactPointsSize);
        for (i = 0; i < contactPointsSize; i++){
            switch(contactPoints[i]->ownerTag){
                case tag_bonus_coin:
                    /* Coins can be gathered by hitting the block under so the coin account will be incremented in the game manager
                    using the wasGathered field of each coin (and not here anymore)
                    */
                    break;
                case tag_bonus_egg:
                    playSoundByID(currMusicManager, 4);
                    if(p_herosInstance->currState == 0){// To avoid a fire tux (state 2) to become a big tux (state 1)
                        if(p_herosInstance->lastDirection == 'r'){
                            changeHerosState(p_herosInstance, 1, 0, 0);
                        }else{
                            changeHerosState(p_herosInstance, 1, 1, 0);
                        }
                    }
                    break;
                case tag_bonus_flower:
                    playSoundByID(currMusicManager, 5);
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
                    // Testing if the heros is above the colliding object
                    if( tux->lastPosY >= contactPoints[i]->posY + contactPoints[i]->height){
                        isAboveSomething = 1;
                        // Bouncing on an enemy
                        if(contactPoints[i]->ownerTag == tag_enemy_fluffy || contactPoints[i]->ownerTag == tag_enemy_bomb || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                            isAboveEnemy = 1;
                        }
                        // Storing each vertical collision
                        if(verticalCollId < verticalCollSize){
                            verticalColl[verticalCollId] = i;
                            verticalCollId += 1;
                        }
                    // Testing if the hero is under the colliding object
                    }else if(tux->lastPosY + tux->height <= contactPoints[i]->posY){
                        // Touching an enemy
                        if(contactPoints[i]->ownerTag == tag_enemy_fluffy || (contactPoints[i]->ownerTag == tag_enemy_bomb && contactPoints[i]->ownerState != state_bomb_primed) || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                            isTouchingEnemy = 1;
                        }
                        // Storing each vertical collision
                        if(verticalCollId < verticalCollSize){
                            verticalColl[verticalCollId] = i;
                            verticalCollId += 1;
                        }
                    }else{// Lateral collision
                        // If left touching
                        if (tux->posX < contactPoints[i]->posX){
                            buffer = contactPoints[i]->posX;
                            if(buffer < rightWall){rightWall = buffer;}
                        }else{ // If right touching
                            buffer = contactPoints[i]->posX + contactPoints[i]->width;
                            if(buffer > leftWall){leftWall = buffer;}
                        }
                        //printf("wall %d, %d, %d\n", leftWall, rightWall, contactPoints[i]->ownerTag);
                        // Touching an enemy
                        if(contactPoints[i]->ownerTag == tag_enemy_fluffy || (contactPoints[i]->ownerTag == tag_enemy_bomb && contactPoints[i]->ownerState != state_bomb_primed) || contactPoints[i]->ownerTag == tag_enemy_iceblock){
                            isTouchingEnemy = 1;
                        }
                    }
            }// End of switch

            // Refresh tux variable
            tux = p_herosInstance->herosColl[p_herosInstance->currState][p_herosInstance->currAction];
        }// End for

        // Right and left collisions
        if(leftWall > tux->posX){ tux->posX = leftWall;}
        if(rightWall < tux->posX + tux->width){ tux->posX = rightWall - tux->width;}

        // Vertical collisions. There are some specific process because vertical collisions depend of lateral ones
        for(i = 0; i < verticalCollId; i++){
            if(contactPoints[verticalColl[i]]->posX + contactPoints[verticalColl[i]]->width > leftWall
               && contactPoints[verticalColl[i]]->posX < rightWall){
                if(contactPoints[verticalColl[i]]->posY < tux->posY){
                    buffer = contactPoints[verticalColl[i]]->posY + contactPoints[verticalColl[i]]->height;
                    if(buffer > tux->posY){
                        tux->posY = buffer;
                    }
                }else{
                    buffer = contactPoints[verticalColl[i]]->posY - tux->height;
                    if(buffer < tux->posY){
                        tux->posY = buffer;
                    }
                    // Stop the current tux's jump
                    p_herosInstance->jumpStartTime = -1;
                    p_herosInstance->jumpDuration = -1;
                }

            }
        }
        // Updating tux sprite position using tux's collider position
        updTuxSpriteLocFromCollLoc(p_herosInstance, p_heros);

        // Tux dies if he fall under a given high
        if(p_herosInstance->posY <= DEAD_LIMIT_Y && !p_herosInstance->isDead){
            heroInstanceDeath(p_herosInstance, p_heros, p_currentTime);
        }

        // Tux becomes smaller when he touches an enemy
        if(isTouchingEnemy && p_herosInstance->godModeDuration <= 0 && !p_herosInstance->isDead){
            // Big or fire tux will become smaller
            if(p_herosInstance->currState == 1 || p_herosInstance->currState == 2){
                playSoundByID(currMusicManager, 6);
                p_herosInstance->godModeDuration = 1000;
                if(p_herosInstance->lastDirection == 'r'){
                    changeHerosState(p_herosInstance, p_herosInstance->currState - 1, 0, 0);
                }else{
                    changeHerosState(p_herosInstance, p_herosInstance->currState - 1, 1, 0);
                }
            }else{
                // Small tux will die
                heroInstanceDeath(p_herosInstance, p_heros, p_currentTime);
            }
        }

        // Firing
        if(p_herosInstance->fireKeyPressed && p_herosInstance->currState == 2 && p_herosInstance->timeBeforeNextShot <= 0 && p_herosInstance->hasReleaseFireKey){
            p_herosInstance->hasReleaseFireKey = 0;
            playSoundByID(currMusicManager, 11);
            if(p_herosInstance->lastDirection == 'r'){
                addBulletInstanceFromLevelMgr(p_collMgr, p_levMgr, p_herosInstance->posX + tux->width + 1
                    , p_herosInstance->posY + tux->height / 2, p_fireBullet, 1);
                changeHerosAction(p_herosInstance, 7, 0);
            }else{
                addBulletInstanceFromLevelMgr(p_collMgr, p_levMgr, p_herosInstance->posX - 1 - p_fireBullet->refColl->width, p_herosInstance->posY + tux->height / 2, p_fireBullet, -1);
                changeHerosAction(p_herosInstance, 8, 0);
            }

            p_herosInstance->timeBeforeNextShot = TIME_BETWEEN_SHOTS;
        }

        // Is the heros is on the ground (or on an enemy)
        if(isAboveSomething){
            p_herosInstance->isTouchingGround = 1;
            // Start a new jump
            if(p_herosInstance->jumpKeyPressed && p_herosInstance->hasReleaseJumpKey){
                playSoundByID(currMusicManager, 8);
                p_herosInstance->hasReleaseJumpKey = 0;
                if(p_herosInstance->lastDirection == 'r'){
                    changeHerosAction(p_herosInstance, 5, 0);
                }else{
                    changeHerosAction(p_herosInstance, 6, 0);
                }
                p_herosInstance->jumpStartTime = p_currentTime;
                p_herosInstance->jumpDuration = 0;
            }else{ // When the heros touch the ground after falling
                if(p_herosInstance->currAction == 5){
                    changeHerosAction(p_herosInstance, 0, 0);
                }
                p_herosInstance->jumpDuration = -1;
            }
        }

        // Bouncing on enemies (a jump with 30% of the max jump high capacity)
        if(isAboveEnemy && p_herosInstance->jumpDuration == -1){
            p_herosInstance->jumpDuration = (int) p_heros->jumpDuration * 0.7;
            p_herosInstance->jumpStartTime = p_currentTime;
            if(p_herosInstance->lastDirection == 'r'){
                changeHerosAction(p_herosInstance, 5, 0);
            }else{
                changeHerosAction(p_herosInstance, 6, 0);
            }
        }

    } // end if tux is alive
}//------------------------------------------------------------------------------------------------------------------------

void displayHeros(HerosManager *p_herosMgr, SDL_Window *p_window, int p_displayedPosX, int p_displayedPosY){
// Display the heros
    SDL_Rect objectPos = {0, 0, 0, 0};
    Collider *tuxColl = p_herosMgr->heroInstance->herosColl[p_herosMgr->heroInstance->currState][p_herosMgr->heroInstance->currAction];
    SDL_Surface *tuxSprite = p_herosMgr->heros->sprites[p_herosMgr->heroInstance->currState][p_herosMgr->heroInstance->currAction][p_herosMgr->heroInstance->currSprite];
    int decalX;
    int decalY;

    objectPos.x = p_displayedPosX;
    objectPos.y = SDL_GetWindowSurface(p_window)->h - p_displayedPosY - tuxColl->height;

    SDL_BlitSurface(tuxSprite, NULL, SDL_GetWindowSurface(p_window), &objectPos);


    // Display tux collider
    if(COLL_DRAWING_ENABLED){
        decalX = (tuxSprite->w - tuxColl->width) / 2;
        decalY = tuxSprite->h - tuxColl->height;
        SDL_Surface * collSurf = SDL_CreateRGBSurface(0, tuxColl->width, tuxColl->height, 32, 0, 0, 0, 0);
        SDL_Rect pos = {0, 0, tuxColl->width, tuxColl->height};
        //SDL_SetAlpha();
        SDL_FillRect(collSurf, &pos, SDL_MapRGBA(collSurf->format, 0, 255, 0, 255));
        SDL_Rect pos2 = {0, 0, 0, 0};
        pos2.x = p_displayedPosX + decalX;
        pos2.y = objectPos.y + decalY;
        SDL_BlitSurface(collSurf, NULL, SDL_GetWindowSurface(p_window), &pos2);
    }
}//------------------------------------------------------------------------------------------------------------------------

void destroyHerosManager(HerosManager *p_herosManager){
// Free HerosManager memory
    if(p_herosManager->heroInstance != NULL){
        destroyHeroInstance(p_herosManager->heroInstance);
    }
    destroyHeros(p_herosManager->heros);
    free(p_herosManager);
}//------------------------------------------------------------------------------------------------------------------------

