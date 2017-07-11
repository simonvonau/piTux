#include "levelManager.h"


LevelManager* initLevelManager(char *levelPath, int levelPathSize){
// Init a new levelManager
    LevelManager *res = malloc(sizeof(LevelManager));
    if(res == NULL){
        reportErreur("initLevelManager():error malloc(...)");
    }
    res->currLevel = NULL;
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void loadLevelByLevelMgr(LevelManager* currLevelMgr, char *path, int pathSize){
// Load a new Level (and free previous Level memory if necessary)
    /*if (currLevelMgr->currLevel != NULL){
        destroyLevel(currLevelMgr->currLevel);
    }*/
    currLevelMgr->currLevel = initLevel(path, pathSize);
}//------------------------------------------------------------------------------------------------------------------------

int getBlockInstanceId(Level *lev, int clicX, int clicY){
// Find a block instance (to put a bonus inside of it)
    int i;

    for(i = 0; i < lev->blockInstancesSize; i++){
        if(clicX >= lev->blockInstances[i]->posX
        && clicX <= lev->blockInstances[i]->coll->width + lev->blockInstances[i]->posX){
            if( clicY >= lev->blockInstances[i]->posY
            && clicY <= lev->blockInstances[i]->posY + lev->blockInstances[i]->coll->height){
                return i;
            }
        }
    }
    return -1;
}//------------------------------------------------------------------------------------------------------------------------

void refreshLevelByLevelManager(Level *currLevel, ColliderManager *currCollMgr, int loopTime, Block **allBlocks, Bonus **allBonus, Enemy **allEnemies, int p_leftLimit, int p_rightLimit, int p_topLimit, int p_bottomLimit){
// Refresh the level (blockInstances,enemyInstances and bonusInstances)
    int i;
    BlockInstance *currBlockI;
    BonusInstance *currBonusI;
    EnemyInstance *currEneI;

    // Refreshing blockInstances
    for(i = 0; i < currLevel->blockInstancesSize; i++){
        currBlockI = currLevel->blockInstances[i];
        if(currBlockI->posX + currBlockI->coll->width > p_leftLimit && currBlockI->posX < p_rightLimit
           && currBlockI->posY + currBlockI->coll->height > p_bottomLimit && currBlockI->posY < p_topLimit){
            refreshBlockInstance(currBlockI, loopTime, *allBlocks[currBlockI->idBlock]);
        }
    }
    // Refreshing bonusInstances
    for(i = 0; i < currLevel->bonusInstancesSize; i++){
        currBonusI = currLevel->bonusInstances[i];
        if(currBonusI->posX + currBonusI->coll->width > p_leftLimit && currBonusI->posX < p_rightLimit
           && currBonusI->posY + currBonusI->coll->height > p_bottomLimit && currBonusI->posY < p_topLimit){
            refreshBonusInstance(currBonusI, loopTime, *allBonus[currBonusI->idBonus]);
        }
    }
    // Refreshing enemyInstances
    for(i = 0; i < currLevel->enemyInstancesSize; i++){
        currEneI = currLevel->enemyInstances[i];
        if(currEneI->posX + currEneI->coll[currEneI->currentActionId]->width > p_leftLimit && currEneI->posX < p_rightLimit
           && currEneI->posY + currEneI->coll[currEneI->currentActionId]->height > p_bottomLimit && currEneI->posY < p_topLimit){
            refreshEnemyInstance(currEneI, loopTime, *allEnemies[currEneI->idEnemy]);
       }
    }
}//------------------------------------------------------------------------------------------------------------------------

void displayLevelByLevelManager(Level *currLevel, SDL_Window *p_window, int isGameMode, int deplaX, int deplaY
                  , Block **allBlocks, Bonus **allBonus, Enemy **allEnemies){
// Display the current level
    int i;
    SDL_Rect objectPos = { 0, 0, 0, 0};
    int margin = COLLIDER_MARGIN;

    // Only display components inside those limits:
    int leftLimit = deplaX - margin;
    int rightLimit = deplaX + margin + SDL_GetWindowSurface(p_window)->w;
    int topLimit = deplaY + margin + SDL_GetWindowSurface(p_window)->h;
    int bottomLimit = deplaY - margin;

    // Displaying BlockInstances
    for( i = 0; i < currLevel->blockInstancesSize; i++){
        if (currLevel->blockInstances[i]->posX >= leftLimit && currLevel->blockInstances[i]->posX <= rightLimit
        &&currLevel->blockInstances[i]->posY >= bottomLimit && currLevel->blockInstances[i]->posY <= topLimit){

            objectPos.x = currLevel->blockInstances[i]->posX - deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->blockInstances[i]->posY
                            - currLevel->blockInstances[i]->coll->height + deplaY;
            SDL_BlitSurface(allBlocks[currLevel->blockInstances[i]->idBlock]->sprites[currLevel->blockInstances[i]->currentActionId][currLevel->blockInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);

            // Displaying the bonus inside a bonus block (levelEditor mode only)
            if( !isGameMode && currLevel->blockInstances[i]->idBonus >= 0 && currLevel->blockInstances[i]->idBonus < currLevel->bonusInstancesSize){
                //*** Need to reset objectPos due to SDL_blit behaviour... (searched on the internet for similar problems...)
                objectPos.x = currLevel->blockInstances[i]->posX - deplaX;
                objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->blockInstances[i]->posY
                            - currLevel->blockInstances[i]->coll->height + deplaY;
                SDL_BlitSurface(allBonus[currLevel->blockInstances[i]->idBonus]->sprites[0][0],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
            }
        }

    }

    // Displaying BonusInstances
    for( i = 0; i < currLevel->bonusInstancesSize; i++){
        if (currLevel->bonusInstances[i]->posX >= leftLimit && currLevel->bonusInstances[i]->posX <= rightLimit
        &&currLevel->bonusInstances[i]->posY >= bottomLimit && currLevel->bonusInstances[i]->posY <= topLimit){
            objectPos.x = currLevel->bonusInstances[i]->posX - deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->bonusInstances[i]->posY
                            - currLevel->bonusInstances[i]->coll->height + deplaY;
            SDL_BlitSurface(allBonus[currLevel->bonusInstances[i]->idBonus]->sprites[currLevel->bonusInstances[i]->currentActionId][currLevel->bonusInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

    // Displaying EnemyInstances
    for( i = 0; i < currLevel->enemyInstancesSize; i++){
        if (currLevel->enemyInstances[i]->posX >= leftLimit && currLevel->enemyInstances[i]->posX <= rightLimit
        &&currLevel->enemyInstances[i]->posY >= bottomLimit && currLevel->enemyInstances[i]->posY <= topLimit){
            objectPos.x = currLevel->enemyInstances[i]->posX - deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->enemyInstances[i]->posY
                            - currLevel->enemyInstances[i]->coll[0]->height + deplaY;
            SDL_BlitSurface(allEnemies[currLevel->enemyInstances[i]->idEnemy]->sprites[currLevel->enemyInstances[i]->currentActionId][currLevel->enemyInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void updateBonusAfterColliding(LevelManager* p_levelManager, ColliderManager *collMgr, int currentTime, int loopTime){
    // Update each bonus after a collision
    Collider **contactPoints;
    int contactPointsSize;
    int i, j;

    for(i = 0; i < p_levelManager->currLevel->bonusInstancesSize; i++){
        getColliderTouching(collMgr, p_levelManager->currLevel->bonusInstances[i]->coll->id, &contactPoints, &contactPointsSize);

        for(j = 0; j < contactPointsSize; j++){
            // If the heros gathered this bonus
            if(contactPoints[j]->ownerTag == TAG_HEROS_TUX){
                p_levelManager->currLevel->bonusInstances[i]->coll->isEnabled = 0;
                switch(p_levelManager->currLevel->bonusInstances[i]->coll->ownerTag){
                    case TAG_BONUS_COIN:
                        p_levelManager->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levelManager->currLevel->bonusInstances[i]->lifeTime = 500;
                        break;
                    case TAG_BONUS_EGG:
                        p_levelManager->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levelManager->currLevel->bonusInstances[i]->lifeTime = -1;
                        break;
                    case TAG_BONUS_FIREFLOWER:
                        p_levelManager->currLevel->bonusInstances[i]->wasGathered = 1;
                        p_levelManager->currLevel->bonusInstances[i]->lifeTime = -1;
                        break;
                }
            }
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void saveLevelByManager(LevelManager* currLevelManager){
// Save the current level
    saveLevel(currLevelManager->currLevel);
}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelManager(LevelManager *currLevelManager){
// Free LevelManager memory
    if (currLevelManager->currLevel != NULL){
        destroyLevel(currLevelManager->currLevel);
    }
    free(currLevelManager);
}//------------------------------------------------------------------------------------------------------------------------

