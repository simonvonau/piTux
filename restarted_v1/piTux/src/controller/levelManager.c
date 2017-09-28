#include "levelManager.h"


LevelManager* initLevelManager(){
// Init a new levelManager
    LevelManager *res = malloc(sizeof(LevelManager));
    if(res == NULL){
        reportErreur("initLevelManager():error malloc(...)");
    }
    res->currLevel = NULL;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void loadLevelByLevelMgr(LevelManager* p_levMgr, char *p_path, int p_pathSize){
// Load a new Level (and free previous Level memory if necessary)
    /*if (p_levMgr->currLevel != NULL){
        destroyLevel(p_levMgr->currLevel);
    }*/
    p_levMgr->currLevel = initLevel(p_path, p_pathSize);
}//------------------------------------------------------------------------------------------------------------------------

int getBlockInstanceId(Level *p_lev, int p_clicX, int p_clicY){
// Find a block instance (to put a bonus inside of it)
    int i;

    for(i = 0; i < p_lev->blockInstancesSize; i++){
        if(p_clicX >= p_lev->blockInstances[i]->posX
        && p_clicX <= p_lev->blockInstances[i]->coll->width + p_lev->blockInstances[i]->posX){
            if( p_clicY >= p_lev->blockInstances[i]->posY
            && p_clicY <= p_lev->blockInstances[i]->posY + p_lev->blockInstances[i]->coll->height){
                return i;
            }
        }
    }
    return -1;
}//------------------------------------------------------------------------------------------------------------------------

void addBulletInstanceFromLevelMgr(ColliderManager *p_collMgr, LevelManager* p_levMgr, int p_posX, int p_posY, FireBullet *p_fb, int p_directionX){
// When tux shoots a bullet
    Collider * tempColl = colliderDeepCopyByColliderManager(p_collMgr, p_fb->refColl);
    tempColl->isEnabled = 1;
    tempColl->posX = p_posX;
    tempColl->posY = p_posY;
    tempColl->lastPosX = p_posX;
    tempColl->lastPosY = p_posY;
    addBulletInstanceToLevel(p_levMgr->currLevel, p_posX, p_posY, tempColl, p_directionX, -1, p_fb->maxLifeTime);
}//------------------------------------------------------------------------------------------------------------------------

void refreshLevelByLevelManager(Level *p_lev, ColliderManager *p_collMgr, int p_loopTime, Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies, FireBullet *p_fireBullet, int p_leftLimit, int p_rightLimit){
// Refresh the level (blockInstances,enemyInstances and bonusInstances)
    int i;
    BlockInstance *currBlockI;
    BonusInstance *currBonusI;
    EnemyInstance *currEneI;
    FireBulletInstance *currFBI;
    p_lev->timeLeft -= p_loopTime;

    // Refreshing blockInstances
    for(i = 0; i < p_lev->blockInstancesSize; i++){
        currBlockI = p_lev->blockInstances[i];
        if(currBlockI->posX + currBlockI->coll->width > p_leftLimit && currBlockI->posX < p_rightLimit){
            refreshBlockInstance(currBlockI, p_loopTime, *p_allBlocks[currBlockI->idBlock]);
        }
    }
    // Refreshing bonusInstances
    for(i = 0; i < p_lev->bonusInstancesSize; i++){
        currBonusI = p_lev->bonusInstances[i];
        if(currBonusI->posX + currBonusI->coll->width > p_leftLimit && currBonusI->posX < p_rightLimit){
            refreshBonusInstance(currBonusI, p_loopTime, *p_allBonus[currBonusI->idBonus]);
        }
    }
    // Refreshing enemyInstances
    for(i = 0; i < p_lev->enemyInstancesSize; i++){
        currEneI = p_lev->enemyInstances[i];
        if(currEneI->posX + currEneI->coll[currEneI->currentActionId]->width > p_leftLimit && currEneI->posX < p_rightLimit){
            refreshEnemyInstance(currEneI, p_loopTime, *p_allEnemies[currEneI->idEnemy]);
       }
    }

    // Refreshing FireBulletInstances
    for(i = 0; i < p_lev->fireBulletInstancesSize; i++){
        currFBI = p_lev->fireBulletInstances[i];
        if(currFBI->posX + currFBI->coll->width > p_leftLimit && currFBI->posX < p_rightLimit){
            refreshFireBulletInstance(currFBI, *p_fireBullet, p_loopTime);
       }
    }
}//------------------------------------------------------------------------------------------------------------------------

void displayLevelByLevelManager(Level *p_lev, SDL_Window *p_window, int p_isGameMode, int p_deplaX, int p_deplaY
                  , Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies, FireBullet *p_fireBullet){
// Display the current level
    int i;
    SDL_Rect objectPos = { 0, 0, 0, 0};
    int margin = COLLIDER_MARGIN;

    // Only display components inside those limits:
    int leftLimit = p_deplaX - margin;
    int rightLimit = p_deplaX + margin + SDL_GetWindowSurface(p_window)->w;
    int topLimit = p_deplaY + margin + SDL_GetWindowSurface(p_window)->h;
    int bottomLimit = p_deplaY - margin;

    // Displaying BlockInstances
    for( i = 0; i < p_lev->blockInstancesSize; i++){
        if ( !p_lev->blockInstances[i]->wasDestroyed
        && p_lev->blockInstances[i]->posX >= leftLimit && p_lev->blockInstances[i]->posX <= rightLimit
        && p_lev->blockInstances[i]->posY >= bottomLimit && p_lev->blockInstances[i]->posY <= topLimit){

            objectPos.x = p_lev->blockInstances[i]->posX - p_deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - p_lev->blockInstances[i]->posY
                            - p_lev->blockInstances[i]->coll->height + p_deplaY;
            SDL_BlitSurface(p_allBlocks[p_lev->blockInstances[i]->idBlock]->sprites[p_lev->blockInstances[i]->currentActionId][p_lev->blockInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);

            // Displaying the bonus inside a bonus block (levelEditor mode only)
            if( !p_isGameMode && p_lev->blockInstances[i]->idBonus >= 0 && p_lev->blockInstances[i]->idBonus < p_lev->bonusInstancesSize){
                //*** Need to reset objectPos due to SDL_blit behaviour... (searched on the internet for similar problems...)
                objectPos.x = p_lev->blockInstances[i]->posX - p_deplaX;
                objectPos.y = SDL_GetWindowSurface(p_window)->h - p_lev->blockInstances[i]->posY
                            - p_lev->blockInstances[i]->coll->height + p_deplaY;
                SDL_BlitSurface(p_allBonus[p_lev->blockInstances[i]->idBonus]->sprites[0][0],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
            }
        }

    }

    // Displaying BonusInstances
    for( i = 0; i < p_lev->bonusInstancesSize; i++){
        if (p_lev->bonusInstances[i]->posX >= leftLimit && p_lev->bonusInstances[i]->posX <= rightLimit
        &&p_lev->bonusInstances[i]->posY >= bottomLimit && p_lev->bonusInstances[i]->posY <= topLimit){
            objectPos.x = p_lev->bonusInstances[i]->posX - p_deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - p_lev->bonusInstances[i]->posY
                            - p_lev->bonusInstances[i]->coll->height + p_deplaY;
            SDL_BlitSurface(p_allBonus[p_lev->bonusInstances[i]->idBonus]->sprites[p_lev->bonusInstances[i]->currentActionId][p_lev->bonusInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

    // Displaying EnemyInstances
    for( i = 0; i < p_lev->enemyInstancesSize; i++){
        if (p_lev->enemyInstances[i]->posX >= leftLimit && p_lev->enemyInstances[i]->posX <= rightLimit
        &&p_lev->enemyInstances[i]->posY >= bottomLimit && p_lev->enemyInstances[i]->posY <= topLimit){
            objectPos.x = p_lev->enemyInstances[i]->posX - p_deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - p_lev->enemyInstances[i]->posY
                            - p_lev->enemyInstances[i]->coll[0]->height + p_deplaY;
            SDL_BlitSurface(p_allEnemies[p_lev->enemyInstances[i]->idEnemy]->sprites[p_lev->enemyInstances[i]->currentActionId][p_lev->enemyInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

    // Displaying FireBulletInstances
    for( i = 0; i < p_lev->fireBulletInstancesSize; i++){
        if (p_lev->fireBulletInstances[i]->posX >= leftLimit && p_lev->fireBulletInstances[i]->posX <= rightLimit
        &&p_lev->fireBulletInstances[i]->posY >= bottomLimit && p_lev->fireBulletInstances[i]->posY <= topLimit){
            objectPos.x = p_lev->fireBulletInstances[i]->posX - p_deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - p_lev->fireBulletInstances[i]->posY
                            - p_lev->fireBulletInstances[i]->coll->height + p_deplaY;
            SDL_BlitSurface(p_fireBullet->sprites[p_lev->fireBulletInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void updateLevelAfterCollisionsDetection(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, int p_minX, int p_maxX, Bonus** p_allBonus, Enemy **p_allEnemies, int p_allBonusSize){
// Update level's items after collisions detection

    updateEnemyAfterColliding(p_levMgr, p_collMgr, p_loopTime, p_allEnemies, p_minX, p_maxX);
    updateBlocksAfterColliding(p_levMgr, p_collMgr, p_currentTime, p_loopTime, p_allBonus, p_allBonusSize, p_minX, p_maxX);
    updateBonusAfterColliding(p_levMgr, p_collMgr, p_currentTime, p_loopTime, p_minX, p_maxX);
    updateFireBulletAfterColliding(p_levMgr, p_collMgr, p_currentTime, p_loopTime);

}//------------------------------------------------------------------------------------------------------------------------

void updateEnemyAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_loopTime, Enemy ** p_allEnemies, int p_minX, int p_maxX){
// Update enemies behaviour after the collision checking
    EnemyInstance * currEnemyInstance;
    int i;
    for(i = 0; i < p_levMgr->currLevel->enemyInstancesSize; i++){
        currEnemyInstance = p_levMgr->currLevel->enemyInstances[i];
        // Only update the displayed enemies
        if(currEnemyInstance->posX + currEnemyInstance->coll[currEnemyInstance->currentActionId]->width > p_minX
           && currEnemyInstance->posX < p_maxX){
            updateEnemyBehaviourAfterCollisionDetection(currEnemyInstance
                             , *p_allEnemies[currEnemyInstance->idEnemy]
                             , p_collMgr
                             , p_loopTime);
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void updateBlocksAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, Bonus ** p_allBonus, int p_allBonusSize, int p_minX, int p_maxX){
// Update each block after a collision
    Collider **contactPoints;
    Collider *newBonusInstanceColl;
    int contactPointsSize;
    int i, j, k;
    BlockInstance* currBlockInst;

    for(i = 0; i < p_levMgr->currLevel->blockInstancesSize; i++){
        currBlockInst = p_levMgr->currLevel->blockInstances[i];

        // Refreshing only the elements displayed on the screen
        if(currBlockInst->posX + currBlockInst->coll->width > p_minX
           && currBlockInst->posX < p_maxX){

            getColliderTouching(p_collMgr, currBlockInst->coll->id, &contactPoints, &contactPointsSize);

            for(j = 0; j < contactPointsSize; j++){
                // If the heros gathered this block
                if(contactPoints[j]->ownerTag == tag_tux){
                    switch(currBlockInst->coll->ownerTag){
                        case tag_block_weak:
                            // When a big tux hit a wood block (collision from under)
                            if(contactPoints[j]->lastPosY + contactPoints[j]->height <= currBlockInst->coll->posY){
                                currBlockInst->coll->ownerState = state_block_hit_up;
                                currBlockInst->currentTime = 0;
                                // If tux is big the wood block will be destroyed
                                if(contactPoints[j]->ownerState == state_tux_big || contactPoints[j]->ownerState == state_tux_fire){
                                    changeBlockAction(currBlockInst, 1, 0);
                                }
                            }
                            break;
                        case tag_block_mystery:
                            // When tux opens a "?" block (collision from under)
                            if(contactPoints[j]->lastPosY + contactPoints[j]->height <= currBlockInst->coll->posY){
                                currBlockInst->coll->ownerState = state_block_hit_up;
                                changeBlockAction(currBlockInst, 1, 0);
                                currBlockInst->currentTime = 0;

                                // When a bonus go out a "?" block
                                if(currBlockInst->idBonus >= 0 && currBlockInst->idBonus < p_allBonusSize){
                                    // Only the small tux can get egg (the "big tux" and the "fire tux" must have a flower)
                                    if(p_allBonus[currBlockInst->idBonus]->refColl->ownerTag == tag_bonus_egg && contactPoints[j]->ownerState != state_tux_small){
                                        for(k = 0; k < p_allBonusSize; k++){
                                            if(p_allBonus[k]->refColl->ownerTag == tag_bonus_flower){
                                                currBlockInst->idBonus = k;
                                            }
                                        }
                                    }
                                    // Making the bonusInstance inside the "?" block
                                    newBonusInstanceColl = createCollider(p_collMgr,
                                                                            p_allBonus[currBlockInst->idBonus]->refColl->width,
                                                                            p_allBonus[currBlockInst->idBonus]->refColl->height,
                                                                            currBlockInst->coll->posX,
                                                                            currBlockInst->coll->posY + currBlockInst->coll->height + 1,
                                                                            1,
                                                                            p_allBonus[currBlockInst->idBonus]->refColl->ownerTag);

                                    addBonusInstanceToLevel(p_levMgr->currLevel, currBlockInst->idBonus, currBlockInst->coll->posX,
                                        currBlockInst->coll->posY + currBlockInst->coll->height, newBonusInstanceColl);
                                    currBlockInst->idBonus = -1;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void updateBonusAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, int p_minX, int p_maxX){
// Update each bonus after a collision
    Collider **contactPoints;
    int contactPointsSize;
    int i, j;
    BonusInstance *currBonusIns;

    for(i = 0; i < p_levMgr->currLevel->bonusInstancesSize; i++){
        currBonusIns = p_levMgr->currLevel->bonusInstances[i];
        if(currBonusIns->posX + currBonusIns->coll->width > p_minX
           && currBonusIns->posX < p_maxX){
            getColliderTouching(p_collMgr, currBonusIns->coll->id, &contactPoints, &contactPointsSize);

            for(j = 0; j < contactPointsSize; j++){
                // If the heros gathered this bonus
                if(contactPoints[j]->ownerTag == tag_tux){
                    currBonusIns->coll->isEnabled = 0;
                    switch(currBonusIns->coll->ownerTag){
                        case tag_bonus_coin:
                            currBonusIns->wasGathered = 1;
                            currBonusIns->lifeTime = 500;
                            break;
                        case tag_bonus_egg:
                            currBonusIns->wasGathered = 1;
                            currBonusIns->lifeTime = -1;
                            break;
                        case tag_bonus_flower:
                            currBonusIns->wasGathered = 1;
                            currBonusIns->lifeTime = -1;
                            break;
                    }
                }else if(contactPoints[j]->ownerTag == tag_block_mystery || contactPoints[j]->ownerTag == tag_block_strong || contactPoints[j]->ownerTag == tag_block_weak){
                    // Reaction of the bonus with its environment
                    if(currBonusIns->coll->ownerTag == tag_bonus_egg){
                        if(currBonusIns->coll->lastPosY < contactPoints[j]->posY + contactPoints[j]->height){
                            if(currBonusIns->coll->lastPosX + currBonusIns->coll->width <= contactPoints[j]->posX){
                                currBonusIns->posX = contactPoints[j]->posX - currBonusIns->coll->width;
                                currBonusIns->direction = - p_levMgr->currLevel->bonusInstances[i]->direction;
                            }else if(p_levMgr->currLevel->bonusInstances[i]->coll->lastPosX >= contactPoints[j]->posX + contactPoints[j]->width){
                                currBonusIns->posX = contactPoints[j]->posX + contactPoints[j]->width;
                                currBonusIns->direction = - currBonusIns->direction;
                            }else{
                                currBonusIns->posY = contactPoints[j]->posY + contactPoints[j]->height;
                            }
                        }else{
                            currBonusIns->posY = contactPoints[j]->posY + contactPoints[j]->height;
                        }
                    }else if(currBonusIns->coll->ownerTag == tag_bonus_flower){
                        currBonusIns->posY = contactPoints[j]->posY + contactPoints[j]->height;
                    }
                }

                // If the heros gathered this coin by hitting the block under it
                if( currBonusIns->coll->isEnabled && currBonusIns->coll->ownerTag == tag_bonus_coin
                && contactPoints[j]->ownerState == state_block_hit_up){
                    currBonusIns->coll->isEnabled = 0;
                    currBonusIns->wasGathered = 1;
                    currBonusIns->lifeTime = 500;
                }
            }
            currBonusIns->coll->posX = currBonusIns->posX;
            currBonusIns->coll->posY = currBonusIns->posY;
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void updateFireBulletAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime){
// Update each fire bullet after a collision
    Collider **contactPoints;
    int contactPointsSize;
    int i, j;

    for(i = 0; i < p_levMgr->currLevel->fireBulletInstancesSize; i++){
        getColliderTouching(p_collMgr, p_levMgr->currLevel->fireBulletInstances[i]->coll->id, &contactPoints, &contactPointsSize);

        for(j = 0; j < contactPointsSize; j++){
            // Destruction of the bullet after touching an enemy
            if(contactPoints[j]->ownerTag == tag_enemy_bomb || contactPoints[j]->ownerTag == tag_enemy_fluffy || contactPoints[j]->ownerTag == tag_enemy_iceblock){
                p_levMgr->currLevel->fireBulletInstances[i]->lifeTimeLeft = -1;
            }
            // Bouncing on the ground
            if(contactPoints[j]->ownerTag == tag_block_mystery || contactPoints[j]->ownerTag == tag_block_strong || contactPoints[j]->ownerTag == tag_block_weak){
                if(p_levMgr->currLevel->fireBulletInstances[i]->coll->lastPosY >= contactPoints[j]->posY + contactPoints[j]->height){
                    p_levMgr->currLevel->fireBulletInstances[i]->directionY = 1;
                    p_levMgr->currLevel->fireBulletInstances[i]->posY = contactPoints[j]->posY + contactPoints[j]->height;
                }else{
                    // If the bullet touches a wall
                    p_levMgr->currLevel->fireBulletInstances[i]->lifeTimeLeft = -1;
                }
            }
        }
        p_levMgr->currLevel->fireBulletInstances[i]->coll->posX = p_levMgr->currLevel->fireBulletInstances[i]->posX;
        p_levMgr->currLevel->fireBulletInstances[i]->coll->posY = p_levMgr->currLevel->fireBulletInstances[i]->posY;
    }

}//------------------------------------------------------------------------------------------------------------------------

void saveLevelByManager(LevelManager* p_levMgr){
// Save the current level
    saveLevel(p_levMgr->currLevel);
}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelManager(LevelManager *p_levMgr){
// Free LevelManager memory
    if (p_levMgr->currLevel != NULL){
        destroyLevel(p_levMgr->currLevel);
    }
    free(p_levMgr);
}//------------------------------------------------------------------------------------------------------------------------

