#include "gameManager.h"


GameManager* initGameManager(char *p_path){
// Init a new Game manager
    GameManager *res = malloc(sizeof(GameManager));

    if(res == NULL){
        reportErreur("initGameManager():error malloc()");
    }

    res->currentGame = initGame(p_path);
    res->translaManager = initTranslationManager(res->currentGame->allTranslationsFile);
    res->collMgr = NULL;
    res->levelManager = initLevelManager();
    res->herosMgr = initHerosManager(res->currentGame->allHerosFile);
    res->allBlocks = initBlockArray(res->currentGame->allBlocksFile, &(res->allBlocksSize));
    res->allBonus = initBonusArray(res->currentGame->allBonusFile, &(res->allBonusSize));
    res->allEnemies = initEnemyArray(res->currentGame->allEnemiesFile, &(res->allEnemiesSize));


    return res;
}//--------------------------------------------------------------------------------------------------------------------

void loadLevelByGameMgr(GameManager *p_gameMgr, char *p_path, int p_pathSize){
// Load a level by a GameManager and add Colliders to it
    int i, j;
    Collider *tempColl;

    //Destroying previous level if exists
    //if(p_gameMgr->levelManager->currLevel != NULL){destroyLevel(p_gameMgr->levelManager->currLevel);}
    loadLevelByLevelMgr(p_gameMgr->levelManager, p_path, p_pathSize);

    // Destroying previous collisions if exists
    //if(p_gameMgr->collMgr != NULL){destroyColliderManager(p_gameMgr->collMgr);}
    p_gameMgr->collMgr = initColliderManager();

    // Init the heros instance
    initHerosInstanceByManager(p_gameMgr->herosMgr);

    // Put Collider on each BlockInstance
    for( i = 0; i < p_gameMgr->levelManager->currLevel->blockInstancesSize; i++){
        p_gameMgr->levelManager->currLevel->blockInstances[i]->coll = colliderDeepCopyByColliderManager(
                p_gameMgr->collMgr
                , p_gameMgr->allBlocks[ p_gameMgr->levelManager->currLevel->blockInstances[i]->idBlock ]->refColl);
        // Update BlockInstance Collider
        tempColl = p_gameMgr->levelManager->currLevel->blockInstances[i]->coll;
        tempColl->ownerTag = p_gameMgr->allBlocks[ p_gameMgr->levelManager->currLevel->blockInstances[i]->idBlock ]->blockType;
        tempColl->isEnabled = 1;
        tempColl->posX = p_gameMgr->levelManager->currLevel->blockInstances[i]->posX;
        tempColl->posY = p_gameMgr->levelManager->currLevel->blockInstances[i]->posY;
    }

    // Put Collider on each BonusInstance
    for( i = 0; i < p_gameMgr->levelManager->currLevel->bonusInstancesSize; i++){
        p_gameMgr->levelManager->currLevel->bonusInstances[i]->coll = colliderDeepCopyByColliderManager(
                p_gameMgr->collMgr
                ,p_gameMgr->allBonus[ p_gameMgr->levelManager->currLevel->bonusInstances[i]->idBonus ]->refColl);
        // Update BonusInstance Collider
        tempColl = p_gameMgr->levelManager->currLevel->bonusInstances[i]->coll;
        tempColl->ownerTag = p_gameMgr->allBonus[ p_gameMgr->levelManager->currLevel->bonusInstances[i]->idBonus ]->bonusType;
        tempColl->isEnabled = 1;
        tempColl->posX = p_gameMgr->levelManager->currLevel->bonusInstances[i]->posX;
        tempColl->posY = p_gameMgr->levelManager->currLevel->bonusInstances[i]->posY;
    }

    // Put Collider on each EnemyInstance
    for( i = 0; i < p_gameMgr->levelManager->currLevel->enemyInstancesSize; i++){
        p_gameMgr->levelManager->currLevel->enemyInstances[i]->coll = colliderArrayDeepCopyByColliderManager(
                p_gameMgr->collMgr
                ,p_gameMgr->allEnemies[ p_gameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->actionColl
                , p_gameMgr->allEnemies[ p_gameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1);
        p_gameMgr->levelManager->currLevel->enemyInstances[i]->collSize =
                p_gameMgr->allEnemies[ p_gameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1;
        // Update EnemyInstance Collider
        for(j = 0; j < p_gameMgr->allEnemies[ p_gameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->spritesSize1; j++){
            tempColl = p_gameMgr->levelManager->currLevel->enemyInstances[i]->coll[j];
            tempColl->ownerTag = p_gameMgr->allEnemies[ p_gameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy ]->enemyType;
            tempColl->posX = p_gameMgr->levelManager->currLevel->enemyInstances[i]->posX;
            tempColl->posY = p_gameMgr->levelManager->currLevel->enemyInstances[i]->posY;
            if (j == p_gameMgr->levelManager->currLevel->enemyInstances[i]->currentActionId){
                tempColl->isEnabled = 1;
            }else{
                tempColl->isEnabled = 0;
            }
        }
    }

    // Put collider on the HerosInstance
    p_gameMgr->herosMgr->heroInstance->herosColl = malloc(sizeof(Collider **) * p_gameMgr->herosMgr->heros->stateSize);
    if(p_gameMgr->herosMgr->heroInstance->herosColl == NULL) {reportErreur("GameManager.loadLevelByGameMgr():Cannot add collider to heroInstance");}
    for( i = 0; i < p_gameMgr->herosMgr->heros->stateSize; i++){
       p_gameMgr->herosMgr->heroInstance->herosColl[i] = colliderArrayDeepCopyByColliderManager(
            p_gameMgr->collMgr
            ,p_gameMgr->herosMgr->heros->herosColl[i]
            ,p_gameMgr->herosMgr->heros->actionSize[i]);
        // Update HerosInstance collider
        for(j = 0; j < p_gameMgr->herosMgr->heros->actionSize[i]; j++){
            tempColl = p_gameMgr->herosMgr->heroInstance->herosColl[i][j];
            tempColl->ownerTag = tag_tux;
            tempColl->posX = p_gameMgr->herosMgr->heroInstance->posX;
            tempColl->posY = p_gameMgr->herosMgr->heroInstance->posY;
            if (i == p_gameMgr->herosMgr->heroInstance->currState && j == p_gameMgr->herosMgr->heroInstance->currAction){
                tempColl->isEnabled = 1;
            }else{
                tempColl->isEnabled = 0;
            }
        }
    }


}//--------------------------------------------------------------------------------------------------------------------

void addElementToLevelByGameMgr(GameManager *p_gameMgr, int p_elemType, int p_elemId, int p_posX, int p_posY){
// Add 1 element to level with auto-position adjustment
    int adjustedPosX;
    int adjustedPosY;
    int blockInstanceId;

    switch(p_elemType){
        case 0: // Block
            adjustedPosX = p_posX - (p_posX % p_gameMgr->allBlocks[p_elemId]->refColl->width);
            adjustedPosY = p_posY - (p_posY % p_gameMgr->allBlocks[p_elemId]->refColl->height);
            // To avoid gap when x < 0 or y < 0
            if(p_posX < 0){adjustedPosX -= p_gameMgr->allBlocks[p_elemId]->refColl->width;}
            if(p_posY < 0){adjustedPosY -= p_gameMgr->allBlocks[p_elemId]->refColl->height;}

            if ( !checkIfBlockInstanceExistHere(p_gameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                addBlockInstanceToLevel(p_gameMgr->levelManager->currLevel, p_elemId, adjustedPosX, adjustedPosY
                    ,colliderDeepCopyByColliderManager(p_gameMgr->collMgr, p_gameMgr->allBlocks[p_elemId]->refColl));
            }
            break;
        case 1: // Bonus
            adjustedPosX = p_posX - (p_posX % p_gameMgr->allBonus[p_elemId]->refColl->width);
            adjustedPosY = p_posY - (p_posY % p_gameMgr->allBonus[p_elemId]->refColl->height);
            // To avoid gap when x < 0 or y < 0
            if(p_posX < 0){adjustedPosX -= p_gameMgr->allBonus[p_elemId]->refColl->width;}
            if(p_posY < 0){adjustedPosY -= p_gameMgr->allBonus[p_elemId]->refColl->height;}

            // If there is already a block here, we put the current bonus inside it
            if( (blockInstanceId = getBlockInstanceId(p_gameMgr->levelManager->currLevel, p_posX, p_posY)) != -1 ){ // Put the bonus in a block
                addBonusToBlock(p_gameMgr->levelManager->currLevel, blockInstanceId, p_elemId);
            }else{ // Just adding the bonus
                if ( !checkIfBonusInstanceExistHere(p_gameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                    addBonusInstanceToLevel(p_gameMgr->levelManager->currLevel, p_elemId, adjustedPosX, adjustedPosY,
                        colliderDeepCopyByColliderManager(p_gameMgr->collMgr, p_gameMgr->allBonus[p_elemId]->refColl));
                }
            }
            break;
        case 2: // enemy
            adjustedPosX = p_posX - p_gameMgr->allEnemies[p_elemId]->actionColl[0]->width / 2;
            adjustedPosY = p_posY - p_gameMgr->allEnemies[p_elemId]->actionColl[0]->height / 2;
            //*** Correction: No need to avoid superposition with enemies!
            //if ( !checkIfEnemyInstanceExistHere(p_gameMgr->levelManager->currLevel, adjustedPosX, adjustedPosY)){ // to avoid superposition
                addEnemyInstanceToLevel(p_gameMgr->levelManager->currLevel, p_elemId, adjustedPosX, adjustedPosY
                    , colliderArrayDeepCopyByColliderManager(p_gameMgr->collMgr
                    , p_gameMgr->allEnemies[p_elemId]->actionColl
                    , p_gameMgr->allEnemies[p_elemId]->spritesSize1)
                    , p_gameMgr->allEnemies[p_elemId]->spritesSize1);
            //}
            break;
    }

}//------------------------------------------------------------------------------------------------------------------------

void removeElementFromLevel(Level *p_lev, int p_clicX, int p_clicY){
// Remove all elements under a click
    int i;

    // Check clicked block(s) existence
    for(i = 0; i < p_lev->blockInstancesSize; i++){
        if(p_clicX >= p_lev->blockInstances[i]->posX
        && p_clicX <= p_lev->blockInstances[i]->coll->width + p_lev->blockInstances[i]->posX){
            if( p_clicY >= p_lev->blockInstances[i]->posY
            && p_clicY <= p_lev->blockInstances[i]->posY + p_lev->blockInstances[i]->coll->height){
                removeBlockInstanceToLevel(p_lev , i);
            }
        }
    }
    // Check clicked bonus existence
    for(i = 0; i < p_lev->bonusInstancesSize; i++){
        if(p_clicX >= p_lev->bonusInstances[i]->posX
        && p_clicX <= p_lev->bonusInstances[i]->coll->width + p_lev->bonusInstances[i]->posX){
            if( p_clicY >= p_lev->bonusInstances[i]->posY
            && p_clicY <= p_lev->bonusInstances[i]->posY + p_lev->bonusInstances[i]->coll->height){
                removeBonusInstanceToLevel(p_lev , i);
            }
        }
    }
    // Check clicked enemy(ies) existence
    for(i = 0; i < p_lev->enemyInstancesSize; i++){
        if(p_clicX >= p_lev->enemyInstances[i]->posX
        && p_clicX <= p_lev->enemyInstances[i]->coll[0]->width + p_lev->enemyInstances[i]->posX){
            if( p_clicY >= p_lev->enemyInstances[i]->posY
            && p_clicY <= p_lev->enemyInstances[i]->posY + p_lev->enemyInstances[i]->coll[0]->height){
                removeEnemyInstanceToLevel(p_lev , i);
                i -= 1;
            }
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void refreshGameByGameManager(GameManager *p_gameMgr, int p_currentTime, int p_loopTime, int p_screenWidth, int p_screenHeight, int p_deplaX, int p_deplaY){
// Refresh the game
/*  This process is made of 3 steps :
    1) Refresh the level (gravity, ...)
    2) Process the collisions
    3) Refresh each object according to its behaviour and the detected collisions
*/
    int i;
    int leftLimit = p_deplaX - COLLIDER_MARGIN;
    int rightLimit = p_deplaX + COLLIDER_MARGIN + p_screenWidth;
    int topLimit = p_deplaY + COLLIDER_MARGIN + p_screenHeight;
    int bottomLimit = p_deplaY - COLLIDER_MARGIN;
    EnemyInstance * currEnemyInstance;

    // Refresh the level (ie. enemy, bonus, blocks)
    refreshLevelByLevelManager(p_gameMgr->levelManager->currLevel, p_gameMgr->collMgr, p_loopTime
                    ,p_gameMgr->allBlocks, p_gameMgr->allBonus, p_gameMgr->allEnemies, leftLimit+100, rightLimit-100, topLimit-100, bottomLimit+100);
    // Refresh Tux
    refreshHerosInstance(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros, p_currentTime, p_loopTime);



    // Check ALL the collision of the game (of course just the displayed part of the level)
    updateCollisions(p_gameMgr->collMgr, leftLimit, rightLimit, bottomLimit, topLimit);



    // Update tux behaviour after the collision checking
    updateHeroBehaviourAfterCollisionDetection(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros, p_gameMgr->collMgr, p_currentTime, p_loopTime);

    updateLevelAfterCollisionsDetection(p_gameMgr->levelManager, p_gameMgr->collMgr, p_currentTime, p_loopTime,
                                        leftLimit, rightLimit, bottomLimit, topLimit, p_gameMgr->allBonus, p_gameMgr->allBonusSize);

    // Update enemies behaviour after the collision checking
    for(i = 0; i < p_gameMgr->levelManager->currLevel->enemyInstancesSize; i++){
        currEnemyInstance = p_gameMgr->levelManager->currLevel->enemyInstances[i];
        // Only update the displayed enemies
        if(currEnemyInstance->posX + currEnemyInstance->coll[currEnemyInstance->currentActionId]->width > leftLimit
           && currEnemyInstance->posX < rightLimit
           && currEnemyInstance->posY + currEnemyInstance->coll[currEnemyInstance->currentActionId]->height > bottomLimit
           && currEnemyInstance->posY < topLimit){
            updateEnemyBehaviourAfterCollisionDetection(currEnemyInstance
                             , *p_gameMgr->allEnemies[currEnemyInstance->idEnemy]
                             , p_gameMgr->collMgr
                             , p_loopTime);
        }
    }

    // Clear the unused memory
    cleanLevelMemory(p_gameMgr);

}//------------------------------------------------------------------------------------------------------------------------

void cleanLevelMemory(GameManager *p_gameMgr){
// Clean the level memory by deallocating dead enemies, gathered bonus, ... and their colliders
    int i, j;
    Level *currLevel = p_gameMgr->levelManager->currLevel;

    // Clean the dead enemies
    for(i = 0; i < currLevel->enemyInstancesSize; i++){
        if(currLevel->enemyInstances[i]->posY <= DEAD_LIMIT_Y || (currLevel->enemyInstances[i]->isDead && currLevel->enemyInstances[i]->lifeTime <= 0)){
            for(j = 0; j < currLevel->enemyInstances[i]->collSize; j++){
                removeCollider(p_gameMgr->collMgr, currLevel->enemyInstances[i]->coll[j]->id, 0);
            }
            removeEnemyInstanceToLevel(currLevel, i);
        }
    }

    // Clean the gathered bonus
    for(i = 0; i < currLevel->bonusInstancesSize; i++){
        if(currLevel->bonusInstances[i]->wasGathered && currLevel->bonusInstances[i]->lifeTime <= 0){
            removeCollider(p_gameMgr->collMgr, currLevel->bonusInstances[i]->coll->id, 0);
            removeBonusInstanceToLevel(currLevel, i);
        }
    }

    // Clean the destroyed blocks
    for(i = 0; i < currLevel->blockInstancesSize; i++){
        if(currLevel->blockInstances[i]->wasDestroyed){
            removeCollider(p_gameMgr->collMgr, currLevel->blockInstances[i]->coll->id, 0);
            removeBlockInstanceToLevel(currLevel, i);
        }
    }


}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelByGameManager(GameManager *p_gameMgr){
// Free Level,Collisions et Colliders memory

    destroyLevel(p_gameMgr->levelManager->currLevel);
    p_gameMgr->levelManager->currLevel = NULL;

    destroyHeroInstanceColliders(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros);
    destroyHeroInstance(p_gameMgr->herosMgr->heroInstance);
    p_gameMgr->herosMgr->heroInstance = NULL;

    destroyColliderManager(p_gameMgr->collMgr);
    p_gameMgr->collMgr = NULL;

}//------------------------------------------------------------------------------------------------------------------------

void destroyGameManager(GameManager *p_gameMgr){
// Free GameManager memory
    int i;
    for( i = 0; i < p_gameMgr->allBlocksSize; i++){
        destroyBlock(p_gameMgr->allBlocks[i]);
    }
    free(p_gameMgr->allBlocks);

    for( i = 0; i < p_gameMgr->allBonusSize; i++){
        destroyBonus(p_gameMgr->allBonus[i]);
    }
    free(p_gameMgr->allBonus);

    destroyHerosManager(p_gameMgr->herosMgr);
    destroyTranslationManager(p_gameMgr->translaManager);
    // Already destroyed at level closure
    //destroyColliderManager(currGameManager->collMgr);
    destroyGame(p_gameMgr->currentGame);

    free(p_gameMgr);
}//--------------------------------------------------------------------------------------------------------------------

