#include "gameManager.h"


GameManager* initGameManager(char *path){
// Init a new Game manager
    GameManager *res = malloc(sizeof(GameManager));

    if(res == NULL){
        reportErreur("initGameManager():error malloc()");
    }

    res->currentGame = initGame(path);
    res->translaManager = initTranslationManager(res->currentGame->allTranslationsFile);
    res->collMgr = NULL;
    res->levelManager = initLevelManager();
    res->herosMgr = initHerosManager(res->currentGame->allHerosFile);
    res->allBlocks = initBlockArray(res->currentGame->allBlocksFile, &(res->allBlocksSize));
    res->allBonus = initBonusArray(res->currentGame->allBonusFile, &(res->allBonusSize));
    res->allEnemies = initEnemyArray(res->currentGame->allEnemiesFile, &(res->allEnemiesSize));


    return res;
}//--------------------------------------------------------------------------------------------------------------------

void loadLevelByGameMgr(GameManager *currGameMgr, char *path, int pathSize){
// Load a level by a GameManager and add Colliders to it
    int i, j;
    Collider *tempColl;

    //Destroying previous level if exists
    //if(currGameMgr->levelManager->currLevel != NULL){destroyLevel(currGameMgr->levelManager->currLevel);}
    loadLevelByLevelMgr(currGameMgr->levelManager, path, pathSize);

    // Destroying previous collisions if exists
    //if(currGameMgr->collMgr != NULL){destroyColliderManager(currGameMgr->collMgr);}
    currGameMgr->collMgr = initColliderManager();

    // Init the heros instance
    initHerosInstanceByManager(currGameMgr->herosMgr);

    // Put Collider on each BlockInstance
    for( i = 0; i < currGameMgr->levelManager->currLevel->blockInstancesSize; i++){
        currGameMgr->levelManager->currLevel->blockInstances[i]->coll = colliderDeepCopyByColliderManager(
                currGameMgr->collMgr
                , currGameMgr->allBlocks[ currGameMgr->levelManager->currLevel->blockInstances[i]->idBlock ]->refColl);
        // Update BlockInstance Collider
        tempColl = currGameMgr->levelManager->currLevel->blockInstances[i]->coll;
        tempColl->ownerTag = currGameMgr->allBlocks[ currGameMgr->levelManager->currLevel->blockInstances[i]->idBlock ]->blockType;
        tempColl->isEnabled = 1;
        tempColl->posX = currGameMgr->levelManager->currLevel->blockInstances[i]->posX;
        tempColl->posY = currGameMgr->levelManager->currLevel->blockInstances[i]->posY;
    }

    // Put Collider on each BonusInstance
    for( i = 0; i < currGameMgr->levelManager->currLevel->bonusInstancesSize; i++){
        currGameMgr->levelManager->currLevel->bonusInstances[i]->coll = colliderDeepCopyByColliderManager(
                currGameMgr->collMgr
                ,currGameMgr->allBonus[ currGameMgr->levelManager->currLevel->bonusInstances[i]->idBonus ]->refColl);
        // Update BonusInstance Collider
        tempColl = currGameMgr->levelManager->currLevel->bonusInstances[i]->coll;
        tempColl->ownerTag = currGameMgr->allBonus[ currGameMgr->levelManager->currLevel->bonusInstances[i]->idBonus ]->bonusType;
        tempColl->isEnabled = 1;
        tempColl->posX = currGameMgr->levelManager->currLevel->bonusInstances[i]->posX;
        tempColl->posY = currGameMgr->levelManager->currLevel->bonusInstances[i]->posY;
    }

    // Put Collider on each EnemyInstance
    for( i = 0; i < currGameMgr->levelManager->currLevel->enemyInstancesSize; i++){
        currGameMgr->levelManager->currLevel->enemyInstances[i]->coll = colliderArrayDeepCopyByColliderManager(
                currGameMgr->collMgr
                ,currGameMgr->allEnemies[ currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->actionColl
                , currGameMgr->allEnemies[ currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1);
        currGameMgr->levelManager->currLevel->enemyInstances[i]->collSize =
                currGameMgr->allEnemies[ currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1;
        // Update EnemyInstance Collider
        for(j = 0; j < currGameMgr->allEnemies[ currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1; j++){
            tempColl = currGameMgr->levelManager->currLevel->enemyInstances[i]->coll[j];
            tempColl->ownerTag = currGameMgr->allEnemies[ currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->enemyType;
            tempColl->posX = currGameMgr->levelManager->currLevel->enemyInstances[i]->posX;
            tempColl->posY = currGameMgr->levelManager->currLevel->enemyInstances[i]->posY;
            if (j == currGameMgr->levelManager->currLevel->enemyInstances[i]->currentActionId){
                tempColl->isEnabled = 1;
            }else{
                tempColl->isEnabled = 0;
            }
        }
    }

    // Put collider on the HerosInstance
    currGameMgr->herosMgr->heroInstance->herosColl = malloc(sizeof(Collider **) * currGameMgr->herosMgr->heros->stateSize);
    if(currGameMgr->herosMgr->heroInstance->herosColl == NULL) {reportErreur("GameManager.loadLevelByGameMgr():Cannot add collider to heroInstance");}
    for( i = 0; i < currGameMgr->herosMgr->heros->stateSize; i++){
       currGameMgr->herosMgr->heroInstance->herosColl[i] = colliderArrayDeepCopyByColliderManager(
            currGameMgr->collMgr
            ,currGameMgr->herosMgr->heros->herosColl[i]
            ,currGameMgr->herosMgr->heros->actionSize[i]);
        // Update HerosInstance collider
        for(j = 0; j < currGameMgr->herosMgr->heros->actionSize[i]; j++){
            tempColl = currGameMgr->herosMgr->heroInstance->herosColl[i][j];
            tempColl->ownerTag = TAG_HEROS_TUX;
            tempColl->posX = currGameMgr->herosMgr->heroInstance->posX;
            tempColl->posY = currGameMgr->herosMgr->heroInstance->posY;
            if (i == currGameMgr->herosMgr->heroInstance->currState && j == currGameMgr->herosMgr->heroInstance->currAction){
                tempColl->isEnabled = 1;
            }else{
                tempColl->isEnabled = 0;
            }
        }
    }


}//--------------------------------------------------------------------------------------------------------------------

void addElementToLevelByGameMgr(GameManager *currGameMgr, int elemType, int elemId, int posX, int posY){
// Add 1 element to level with auto-position adjustment
    int adjustedPosX;
    int adjustedPosY;
    int blockInstanceId;

    switch(elemType){
        case 0: // Block
            adjustedPosX = posX - (posX % currGameMgr->allBlocks[elemId]->refColl->width);
            adjustedPosY = posY - (posY % currGameMgr->allBlocks[elemId]->refColl->height);
            // To avoid gap when x < 0 or y < 0
            if(posX < 0){adjustedPosX -= currGameMgr->allBlocks[elemId]->refColl->width;}
            if(posY < 0){adjustedPosY -= currGameMgr->allBlocks[elemId]->refColl->height;}

            if ( !checkIfBlockInstanceExistHere(currGameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                addBlockInstanceToLevel(currGameMgr->levelManager->currLevel, elemId, adjustedPosX, adjustedPosY
                    ,colliderDeepCopyByColliderManager(currGameMgr->collMgr, currGameMgr->allBlocks[elemId]->refColl));
            }
            break;
        case 1: // Bonus
            adjustedPosX = posX - (posX % currGameMgr->allBonus[elemId]->refColl->width);
            adjustedPosY = posY - (posY % currGameMgr->allBonus[elemId]->refColl->height);
            // To avoid gap when x < 0 or y < 0
            if(posX < 0){adjustedPosX -= currGameMgr->allBonus[elemId]->refColl->width;}
            if(posY < 0){adjustedPosY -= currGameMgr->allBonus[elemId]->refColl->height;}

            // If there is already a block here, we put the current bonus inside it
            if( (blockInstanceId = getBlockInstanceId(currGameMgr->levelManager->currLevel, posX, posY)) != -1 ){ // Put the bonus in a block
                addBonusToBlock(currGameMgr->levelManager->currLevel, blockInstanceId, elemId);
            }else{ // Just adding the bonus
                if ( !checkIfBonusInstanceExistHere(currGameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                    addBonusInstanceToLevel(currGameMgr->levelManager->currLevel, elemId, adjustedPosX, adjustedPosY,
                        colliderDeepCopyByColliderManager(currGameMgr->collMgr, currGameMgr->allBonus[elemId]->refColl));
                }
            }
            break;
        case 2: // enemy
            adjustedPosX = posX - currGameMgr->allEnemies[elemId]->actionColl[0]->width / 2;
            adjustedPosY = posY - currGameMgr->allEnemies[elemId]->actionColl[0]->height / 2;
            //*** Correction: No need to avoid superposition with enemies!
            //if ( !checkIfEnemyInstanceExistHere(currGameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                addEnemyInstanceToLevel(currGameMgr->levelManager->currLevel, elemId, adjustedPosX, adjustedPosY
                    , colliderArrayDeepCopyByColliderManager(currGameMgr->collMgr
                    , currGameMgr->allEnemies[elemId]->actionColl
                    , currGameMgr->allEnemies[elemId]->spritesSize1)
                    , currGameMgr->allEnemies[elemId]->spritesSize1);
            //}
            break;
    }

}//------------------------------------------------------------------------------------------------------------------------

void removeElementFromLevel(Level *lev, int clicX, int clicY){
// Remove all elements under a click
    int i;

    // Check clicked block(s) existence
    for(i = 0; i < lev->blockInstancesSize; i++){
        if(clicX >= lev->blockInstances[i]->posX
        && clicX <= lev->blockInstances[i]->coll->width + lev->blockInstances[i]->posX){
            if( clicY >= lev->blockInstances[i]->posY
            && clicY <= lev->blockInstances[i]->posY + lev->blockInstances[i]->coll->height){
                removeBlockInstanceToLevel(lev , i);
            }
        }
    }
    // Check clicked bonus existence
    for(i = 0; i < lev->bonusInstancesSize; i++){
        if(clicX >= lev->bonusInstances[i]->posX
        && clicX <= lev->bonusInstances[i]->coll->width + lev->bonusInstances[i]->posX){
            if( clicY >= lev->bonusInstances[i]->posY
            && clicY <= lev->bonusInstances[i]->posY + lev->bonusInstances[i]->coll->height){
                removeBonusInstanceToLevel(lev , i);
            }
        }
    }
    // Check clicked enemy(ies) existence
    for(i = 0; i < lev->enemyInstancesSize; i++){
        if(clicX >= lev->enemyInstances[i]->posX
        && clicX <= lev->enemyInstances[i]->coll[0]->width + lev->enemyInstances[i]->posX){
            if( clicY >= lev->enemyInstances[i]->posY
            && clicY <= lev->enemyInstances[i]->posY + lev->enemyInstances[i]->coll[0]->height){
                removeEnemyInstanceToLevel(lev , i);
                i -= 1;
            }
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void refreshGameByGameManager(GameManager *currGameMgr, int currentTime, int loopTime, int screenWidth, int screenHeight, int deplaX, int deplaY){
// Refresh game
    int i;
    int leftLimit = deplaX - COLLIDER_MARGIN;
    int rightLimit = deplaX + COLLIDER_MARGIN + screenWidth;
    int topLimit = deplaY + COLLIDER_MARGIN + screenHeight;
    int bottomLimit = deplaY - COLLIDER_MARGIN;
    EnemyInstance * currEnemyInstance;

    // Refresh the level (ie. enemy, bonus, blocks)
    refreshLevelByLevelManager(currGameMgr->levelManager->currLevel, currGameMgr->collMgr, loopTime
                    ,currGameMgr->allBlocks, currGameMgr->allBonus, currGameMgr->allEnemies, leftLimit+100, rightLimit-100, topLimit-100, bottomLimit+100);
    // Refresh Tux
    refreshHeroInstance(currGameMgr->herosMgr->heroInstance, currGameMgr->herosMgr->heros, currentTime, loopTime);

    // Check ALL the collision of the game (of course just the displayed part of the level)
    updateCollisions(currGameMgr->collMgr, leftLimit, rightLimit, bottomLimit, topLimit);

    updateHeroBehaviourAfterCollisionDetection(currGameMgr->herosMgr->heroInstance, currGameMgr->herosMgr->heros, currGameMgr->collMgr, currentTime, loopTime);

    // Update the bonus after collision checking
    updateBonusAfterColliding(currGameMgr->levelManager, currGameMgr->collMgr, currentTime, loopTime);

    // Update enemies behaviour after the collision checking
    for(i = 0; i < currGameMgr->levelManager->currLevel->enemyInstancesSize; i++){
        currEnemyInstance = currGameMgr->levelManager->currLevel->enemyInstances[i];
        // Only update the displayed enemies
        if(currEnemyInstance->posX + currEnemyInstance->coll[currEnemyInstance->currentActionId]->width > leftLimit
           && currEnemyInstance->posX < rightLimit
           && currEnemyInstance->posY + currEnemyInstance->coll[currEnemyInstance->currentActionId]->height > bottomLimit
           && currEnemyInstance->posY < topLimit){
            updateEnemyBehaviourAfterCollisionDetection(currEnemyInstance
                             , *currGameMgr->allEnemies[currEnemyInstance->idEnemy]
                             , currGameMgr->collMgr
                             , loopTime);
        }
    }

    // Clear the unused memory
    cleanLevelMemory(currGameMgr);

}//------------------------------------------------------------------------------------------------------------------------

void cleanLevelMemory(GameManager *currGameManager){
// Clean the level memory by deallocating dead enemies, gathered bonus, ... and their colliders
    int i, j;
    Level *currLevel = currGameManager->levelManager->currLevel;

    // Clean the dead enemies
    for(i = 0; i < currLevel->enemyInstancesSize; i++){
        if(currLevel->enemyInstances[i]->posY <= DEAD_LIMIT_Y || (currLevel->enemyInstances[i]->isDead && currLevel->enemyInstances[i]->lifeTime <= 0)){
            for(j = 0; j < currLevel->enemyInstances[i]->collSize; j++){
                removeCollider(currGameManager->collMgr, currLevel->enemyInstances[i]->coll[j]->id, 0);
            }
            removeEnemyInstanceToLevel(currLevel, i);
        }
    }

    // Clean the gathered bonus
    for(i = 0; i < currLevel->bonusInstancesSize; i++){
        if(currLevel->bonusInstances[i]->wasGathered && currLevel->bonusInstances[i]->lifeTime <= 0){
            removeCollider(currGameManager->collMgr, currLevel->bonusInstances[i]->coll->id, 0);
            removeBonusInstanceToLevel(currLevel, i);
        }
    }


}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelByGameManager(GameManager *currGameManager){
// Free Level,Collisions et Colliders memory

    destroyLevel(currGameManager->levelManager->currLevel);
    currGameManager->levelManager->currLevel = NULL;

    destroyHeroInstanceColliders(currGameManager->herosMgr->heroInstance, currGameManager->herosMgr->heros);
    destroyHeroInstance(currGameManager->herosMgr->heroInstance);
    currGameManager->herosMgr->heroInstance = NULL;

    destroyColliderManager(currGameManager->collMgr);
    currGameManager->collMgr = NULL;

}//------------------------------------------------------------------------------------------------------------------------

void destroyGameManager(GameManager *currGameManager){
// Free GameManager memory
    int i;
    for( i = 0; i < currGameManager->allBlocksSize; i++){
        destroyBlock(currGameManager->allBlocks[i]);
    }
    free(currGameManager->allBlocks);

    for( i = 0; i < currGameManager->allBonusSize; i++){
        destroyBonus(currGameManager->allBonus[i]);
    }
    free(currGameManager->allBonus);

    destroyHerosManager(currGameManager->herosMgr);
    destroyTranslationManager(currGameManager->translaManager);
    // Already destroyed at level closure
    //destroyColliderManager(currGameManager->collMgr);
    destroyGame(currGameManager->currentGame);

    free(currGameManager);
}//--------------------------------------------------------------------------------------------------------------------

