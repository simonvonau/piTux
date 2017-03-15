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

void refreshLevelByLevelManager(Level *currLevel, ColliderManager *currCollMgr, int loopTime, Block **allBlocks, Bonus **allBonus, Enemy **allEnemies){
// Refresh the level (blockInstances,enemyInstances and bonusInstances)
    int i;

    // Refreshing blockInstances
    for(i = 0; i < currLevel->blockInstancesSize; i++){
        refreshBlockInstance(currLevel->blockInstances[i], loopTime, *allBlocks[currLevel->blockInstances[i]->idBlock]);
    }
    // Refreshing bonusInstances
    for(i = 0; i < currLevel->bonusInstancesSize; i++){
        refreshBonusInstance(currLevel->bonusInstances[i], loopTime, *allBonus[currLevel->bonusInstances[i]->idBonus]);
    }
    // Refreshing enemyInstances
    for(i = 0; i < currLevel->enemyInstancesSize; i++){
        updateEnemyBehaviour(currLevel->enemyInstances[i], *allEnemies[currLevel->enemyInstances[i]->idEnemy], currCollMgr, loopTime);
        refreshEnemyInstance(currLevel->enemyInstances[i], loopTime, *allEnemies[currLevel->enemyInstances[i]->idEnemy]);
    }
}//------------------------------------------------------------------------------------------------------------------------

void displayLevelByLevelManager(Level *currLevel, SDL_Window *p_window, int isGameMode, int deplaX, int deplaY
                  , Block **allBlocks, Bonus **allBonus, Enemy **allEnemies){
// Display the current level
    int i;
    SDL_Rect objectPos = { 0, 0, 0, 0};
    int margin = 200;

    // Only display components inside those limits:
    int leftLimit = -deplaX - margin;
    int rightLimit = -deplaX + margin + SDL_GetWindowSurface(p_window)->w;
    int topLimit = deplaY + margin + SDL_GetWindowSurface(p_window)->h;
    int bottomLimit = deplaY - margin;

    // Displaying BlockInstances
    for( i = 0; i < currLevel->blockInstancesSize; i++){
        if (currLevel->blockInstances[i]->posX >= leftLimit && currLevel->blockInstances[i]->posX <= rightLimit
        &&currLevel->blockInstances[i]->posY >= bottomLimit && currLevel->blockInstances[i]->posY <= topLimit){

            objectPos.x = currLevel->blockInstances[i]->posX + deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->blockInstances[i]->posY
                            - currLevel->blockInstances[i]->coll->height + deplaY;
            SDL_BlitSurface(allBlocks[currLevel->blockInstances[i]->idBlock]->sprites[currLevel->blockInstances[i]->currentActionId][currLevel->blockInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);

            // Displaying the bonus inside a bonus block (levelEditor mode only)
            if( !isGameMode && currLevel->blockInstances[i]->idBonus >= 0 && currLevel->blockInstances[i]->idBonus < currLevel->bonusInstancesSize){
                //*** Need to reset objectPos due to SDL_blit behaviour... (searched on the internet for similar problems...)
                objectPos.x = currLevel->blockInstances[i]->posX + deplaX;
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
            objectPos.x = currLevel->bonusInstances[i]->posX + deplaX;
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
            objectPos.x = currLevel->enemyInstances[i]->posX + deplaX;
            objectPos.y = SDL_GetWindowSurface(p_window)->h - currLevel->enemyInstances[i]->posY
                            - currLevel->enemyInstances[i]->coll[0]->height + deplaY;
            SDL_BlitSurface(allEnemies[currLevel->enemyInstances[i]->idEnemy]->sprites[currLevel->enemyInstances[i]->currentActionId][currLevel->enemyInstances[i]->currentSpriteId],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
        }
    }

}//------------------------------------------------------------------------------------------------------------------------

void saveLevelByManager(LevelManager* currLevelManager){
// Save the current level
    saveLevel(currLevelManager->currLevel);
}//------------------------------------------------------------------------------------------------------------------------

void gatherBonusByCollider(LevelManager *currLevelMgr, Collider *currColl){
//

}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelManager(LevelManager *currLevelManager){
// Free LevelManager memory
    if (currLevelManager->currLevel != NULL){
        destroyLevel(currLevelManager->currLevel);
    }
    free(currLevelManager);
}//------------------------------------------------------------------------------------------------------------------------

