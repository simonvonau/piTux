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

void refreshLevelByLevelManager(Level *p_lev, ColliderManager *p_collMgr, int p_loopTime, Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies, int p_leftLimit, int p_rightLimit, int p_topLimit, int p_bottomLimit){
// Refresh the level (blockInstances,enemyInstances and bonusInstances)
    int i;
    BlockInstance *currBlockI;
    BonusInstance *currBonusI;
    EnemyInstance *currEneI;

    // Refreshing blockInstances
    for(i = 0; i < p_lev->blockInstancesSize; i++){
        currBlockI = p_lev->blockInstances[i];
        if(currBlockI->posX + currBlockI->coll->width > p_leftLimit && currBlockI->posX < p_rightLimit
           && currBlockI->posY + currBlockI->coll->height > p_bottomLimit && currBlockI->posY < p_topLimit){
            refreshBlockInstance(currBlockI, p_loopTime, *p_allBlocks[currBlockI->idBlock]);
        }
    }
    // Refreshing bonusInstances
    for(i = 0; i < p_lev->bonusInstancesSize; i++){
        currBonusI = p_lev->bonusInstances[i];
        if(currBonusI->posX + currBonusI->coll->width > p_leftLimit && currBonusI->posX < p_rightLimit
           && currBonusI->posY + currBonusI->coll->height > p_bottomLimit && currBonusI->posY < p_topLimit){
            refreshBonusInstance(currBonusI, p_loopTime, *p_allBonus[currBonusI->idBonus]);
        }
    }
    // Refreshing enemyInstances
    for(i = 0; i < p_lev->enemyInstancesSize; i++){
        currEneI = p_lev->enemyInstances[i];
        if(currEneI->posX + currEneI->coll[currEneI->currentActionId]->width > p_leftLimit && currEneI->posX < p_rightLimit
           && currEneI->posY + currEneI->coll[currEneI->currentActionId]->height > p_bottomLimit && currEneI->posY < p_topLimit){
            refreshEnemyInstance(currEneI, p_loopTime, *p_allEnemies[currEneI->idEnemy]);
       }
    }
}//------------------------------------------------------------------------------------------------------------------------

void displayLevelByLevelManager(Level *p_lev, SDL_Window *p_window, int p_isGameMode, int p_deplaX, int p_deplaY
                  , Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies){
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

}//------------------------------------------------------------------------------------------------------------------------

void updateLevelAfterCollisionsDetection(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, int p_minX, int p_maxX, int p_minY, int p_maxY, Bonus** p_allBonus, int p_allBonusSize){
    // Update level's items after collisions detection

    updateBlocksAfterColliding(p_levMgr, p_collMgr, p_currentTime, p_loopTime, p_allBonus, p_allBonusSize);
    updateBonusAfterColliding(p_levMgr, p_collMgr, p_currentTime, p_loopTime);

}//------------------------------------------------------------------------------------------------------------------------

void updateBlocksAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, Bonus ** p_allBonus, int p_allBonusSize){
    // Update each block after a collision
    Collider **contactPoints;
    Collider *newBonusInstanceColl;
    int contactPointsSize;
    int i, j, k;

    for(i = 0; i < p_levMgr->currLevel->blockInstancesSize; i++){
        getColliderTouching(p_collMgr, p_levMgr->currLevel->blockInstances[i]->coll->id, &contactPoints, &contactPointsSize);

        for(j = 0; j < contactPointsSize; j++){
            // If the heros gathered this block
            if(contactPoints[j]->ownerTag == tag_tux){
                switch(p_levMgr->currLevel->blockInstances[i]->coll->ownerTag){
                    case tag_block_weak:
                        // When a big tux hit a wood block (collision from under)
                        if(contactPoints[j]->lastPosY + contactPoints[j]->height <= p_levMgr->currLevel->blockInstances[i]->coll->posY){
                            p_levMgr->currLevel->blockInstances[i]->coll->ownerState = state_block_hit_up;
                            p_levMgr->currLevel->blockInstances[i]->currentTime = 0;
                            // If tux is big the wood block will be destroyed
                            if(contactPoints[j]->ownerState == state_tux_big || contactPoints[j]->ownerState == state_tux_fire){
                                changeBlockAction(p_levMgr->currLevel->blockInstances[i], 1, 0);
                            }
                        }
                        break;
                    case tag_block_mystery:
                        // When tux opens a "?" block (collision from under)
                        if(contactPoints[j]->lastPosY + contactPoints[j]->height <= p_levMgr->currLevel->blockInstances[i]->coll->posY){
                            p_levMgr->currLevel->blockInstances[i]->coll->ownerState = state_block_hit_up;
                            changeBlockAction(p_levMgr->currLevel->blockInstances[i], 1, 0);
                            p_levMgr->currLevel->blockInstances[i]->currentTime = 0;

                            // When a bonus go out a "?" block
                            if(p_levMgr->currLevel->blockInstances[i]->idBonus >= 0 && p_levMgr->currLevel->blockInstances[i]->idBonus < p_allBonusSize){
                                // Only the small tux can get egg (the "big tux" and the "fire tux" must have a flower)
                                if(p_allBonus[p_levMgr->currLevel->blockInstances[i]->idBonus]->refColl->ownerTag == tag_bonus_egg && contactPoints[j]->ownerState != state_tux_small){
                                    for(k = 0; k < p_allBonusSize; k++){
                                        if(p_allBonus[k]->refColl->ownerTag == tag_bonus_flower){
                                            p_levMgr->currLevel->blockInstances[i]->idBonus = k;
                                        }
                                    }
                                }
                                newBonusInstanceColl = createCollider(p_collMgr,
                                                                        p_allBonus[p_levMgr->currLevel->blockInstances[i]->idBonus]->refColl->width,
                                                                        p_allBonus[p_levMgr->currLevel->blockInstances[i]->idBonus]->refColl->height,
                                                                        p_levMgr->currLevel->blockInstances[i]->coll->posX,
                                                                        p_levMgr->currLevel->blockInstances[i]->coll->posY + p_levMgr->currLevel->blockInstances[i]->coll->height + 1,
                                                                        1,
                                                                        p_allBonus[p_levMgr->currLevel->blockInstances[i]->idBonus]->refColl->ownerTag);

                                addBonusInstanceToLevel(p_levMgr->currLevel, p_levMgr->currLevel->blockInstances[i]->idBonus, p_levMgr->currLevel->blockInstances[i]->coll->posX,
                                    p_levMgr->currLevel->blockInstances[i]->coll->posY + p_levMgr->currLevel->blockInstances[i]->coll->height, newBonusInstanceColl);
                                p_levMgr->currLevel->blockInstances[i]->idBonus = -1;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void updateBonusAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime){
    // Update each bonus after a collision
    Collider **contactPoints;
    int contactPointsSize;
    int i, j;

    for(i = 0; i < p_levMgr->currLevel->bonusInstancesSize; i++){
        getColliderTouching(p_collMgr, p_levMgr->currLevel->bonusInstances[i]->coll->id, &contactPoints, &contactPointsSize);

        for(j = 0; j < contactPointsSize; j++){
            // If the heros gathered this bonus
            if(contactPoints[j]->ownerTag == tag_tux){
                p_levMgr->currLevel->bonusInstances[i]->coll->isEnabled = 0;
                switch(p_levMgr->currLevel->bonusInstances[i]->coll->ownerTag){
                    case tag_bonus_coin:
                        p_levMgr->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levMgr->currLevel->bonusInstances[i]->lifeTime = 500;
                        break;
                    case tag_bonus_egg:
                        p_levMgr->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levMgr->currLevel->bonusInstances[i]->lifeTime = -1;
                        break;
                    case tag_bonus_flower:
                        p_levMgr->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levMgr->currLevel->bonusInstances[i]->lifeTime = -1;
                        break;
                }
            }else if(contactPoints[j]->ownerTag == tag_block_mystery || contactPoints[j]->ownerTag == tag_block_strong || contactPoints[j]->ownerTag == tag_block_weak){
                // Reaction of the bonus with its environment
                if(p_levMgr->currLevel->bonusInstances[i]->coll->ownerTag == tag_bonus_egg){
                    if(p_levMgr->currLevel->bonusInstances[i]->coll->lastPosY < contactPoints[j]->posY + contactPoints[j]->height){
                        if(p_levMgr->currLevel->bonusInstances[i]->coll->lastPosX + p_levMgr->currLevel->bonusInstances[i]->coll->width <= contactPoints[j]->posX){
                            p_levMgr->currLevel->bonusInstances[i]->posX = contactPoints[j]->posX - p_levMgr->currLevel->bonusInstances[i]->coll->width;
                            p_levMgr->currLevel->bonusInstances[i]->direction = - p_levMgr->currLevel->bonusInstances[i]->direction;
                        }else if(p_levMgr->currLevel->bonusInstances[i]->coll->lastPosX >= contactPoints[j]->posX + contactPoints[j]->width){
                            p_levMgr->currLevel->bonusInstances[i]->posX = contactPoints[j]->posX + contactPoints[j]->width;
                            p_levMgr->currLevel->bonusInstances[i]->direction = - p_levMgr->currLevel->bonusInstances[i]->direction;
                        }else{
                            p_levMgr->currLevel->bonusInstances[i]->posY = contactPoints[j]->posY + contactPoints[j]->height;
                        }
                    }else{
                        p_levMgr->currLevel->bonusInstances[i]->posY = contactPoints[j]->posY + contactPoints[j]->height;
                    }
                }else if(p_levMgr->currLevel->bonusInstances[i]->coll->ownerTag == tag_bonus_flower){
                    p_levMgr->currLevel->bonusInstances[i]->posY = contactPoints[j]->posY + contactPoints[j]->height;
                }
            }

            // If the heros gathered this coin by hitting the block under it
            if( p_levMgr->currLevel->bonusInstances[i]->coll->isEnabled && p_levMgr->currLevel->bonusInstances[i]->coll->ownerTag == tag_bonus_coin
            && contactPoints[j]->ownerState == state_block_hit_up){
                p_levMgr->currLevel->bonusInstances[i]->coll->isEnabled = 0;
                p_levMgr->currLevel->bonusInstances[i]->wasGathered = 1;
                p_levMgr->currLevel->bonusInstances[i]->lifeTime = 500;
            }
        }
        /*p_levMgr->currLevel->bonusInstances[i]->coll->lastPosX = p_levMgr->currLevel->bonusInstances[i]->coll->posX;
        p_levMgr->currLevel->bonusInstances[i]->coll->lastPosY = p_levMgr->currLevel->bonusInstances[i]->coll->posY;*/
        p_levMgr->currLevel->bonusInstances[i]->coll->posX = p_levMgr->currLevel->bonusInstances[i]->posX;
        p_levMgr->currLevel->bonusInstances[i]->coll->posY = p_levMgr->currLevel->bonusInstances[i]->posY;
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

