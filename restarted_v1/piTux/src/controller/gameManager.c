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
    res->levIconMgr = initLevelIconManager(res->currentGame->allLevelIconsFile);
    res->allBlocks = initBlockArray(res->currentGame->allBlocksFile, &(res->allBlocksSize));
    res->allBonus = initBonusArray(res->currentGame->allBonusFile, &(res->allBonusSize));
    res->allEnemies = initEnemyArray(res->currentGame->allEnemiesFile, &(res->allEnemiesSize));
    res->fireBullet = initFireBullet(res->currentGame->fireBulletFile);


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
    //initHerosInstanceByManager(p_gameMgr->herosMgr);

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
    p_gameMgr->herosMgr->heroInstance->herosColl = malloc(sizeof(Collider *) * p_gameMgr->herosMgr->heros->stateSize);
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
    SDL_Surface *currSprite;

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
            currSprite = p_gameMgr->allEnemies[p_elemId]->sprites[0][0];
            adjustedPosX = p_posX - currSprite->w / 2;
            adjustedPosY = p_posY - currSprite->h / 2;
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

void removeElementFromLevelByGameMgr(GameManager *p_gameMgr, int p_clicX, int p_clicY){
// Remove all elements under a click
    int i;
    Level * lev = p_gameMgr->levelManager->currLevel;
    SDL_Surface *currSprite;

    // Check clicked block(s) existence
    for(i = 0; i < lev->blockInstancesSize; i++){
        if(p_clicX >= lev->blockInstances[i]->posX
        && p_clicX <= lev->blockInstances[i]->coll->width + lev->blockInstances[i]->posX){
            if( p_clicY >= lev->blockInstances[i]->posY
            && p_clicY <= lev->blockInstances[i]->posY + lev->blockInstances[i]->coll->height){
                removeBlockInstanceToLevel(lev , i);
            }
        }
    }
    // Check clicked bonus existence
    for(i = 0; i < lev->bonusInstancesSize; i++){
        if(p_clicX >= lev->bonusInstances[i]->posX
        && p_clicX <= lev->bonusInstances[i]->coll->width + lev->bonusInstances[i]->posX){
            if( p_clicY >= lev->bonusInstances[i]->posY
            && p_clicY <= lev->bonusInstances[i]->posY + lev->bonusInstances[i]->coll->height){
                removeBonusInstanceToLevel(lev , i);
            }
        }
    }

    // Check clicked enemy(ies) existence
    for(i = 0; i < lev->enemyInstancesSize; i++){
        currSprite = p_gameMgr->allEnemies[lev->enemyInstances[i]->idEnemy]->sprites[0][0];
        if(p_clicX >= lev->enemyInstances[i]->posX
        && p_clicX <= currSprite->w + lev->enemyInstances[i]->posX){
            if( p_clicY >= lev->enemyInstances[i]->posY
            && p_clicY <= lev->enemyInstances[i]->posY + currSprite->h){
                removeEnemyInstanceToLevel(lev , i);
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
    int leftLimit = p_deplaX - COLLIDER_MARGIN;
    int rightLimit = p_deplaX + COLLIDER_MARGIN + p_screenWidth;

    // Refresh the level (ie. enemy, bonus, blocks)
    refreshLevelByLevelManager(p_gameMgr->levelManager->currLevel, p_gameMgr->collMgr, p_loopTime
                    ,p_gameMgr->allBlocks, p_gameMgr->allBonus, p_gameMgr->allEnemies, p_gameMgr->fireBullet, leftLimit+100, rightLimit-100);
    // Refresh Tux
    refreshHerosInstance(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros, p_currentTime, p_loopTime);



    // Check ALL the collision of the game (of course just the displayed part of the level)
    updateCollisions(p_gameMgr->collMgr, leftLimit, rightLimit, -2000, 2000);

    // Update all the level items (blocks, bonus, enemies) behaviour after the collision checking
    updateLevelAfterCollisionsDetection(p_gameMgr->levelManager, p_gameMgr->collMgr, p_currentTime, p_loopTime,
                                        leftLimit, rightLimit, p_gameMgr->allBonus, p_gameMgr->allEnemies, p_gameMgr->allBonusSize);

    // Update tux behaviour after the collision checking
    updateHeroBehaviourAfterCollisionDetection(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros, p_gameMgr->collMgr, p_currentTime, p_loopTime, p_gameMgr->levelManager, p_gameMgr->fireBullet);



    // Clear the unused memory by removing useless items from the level
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
            // Adding a coin to the heros
            if(currLevel->bonusInstances[i]->coll->ownerTag == tag_bonus_coin){
                gatherCoin(p_gameMgr->herosMgr->heroInstance);
            }

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

    // Clean the bullets
    for(i = 0; i < currLevel->fireBulletInstancesSize; i++){
        if(currLevel->fireBulletInstances[i]->lifeTimeLeft <= 0){
            removeCollider(p_gameMgr->collMgr, currLevel->fireBulletInstances[i]->coll->id, 0);
            removeFireBulletInstanceToLevel(currLevel, i);
        }
    }


}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelByGameManager(GameManager *p_gameMgr){
// Free Level,Collisions et Colliders memory

    destroyLevel(p_gameMgr->levelManager->currLevel);
    p_gameMgr->levelManager->currLevel = NULL;

    destroyHeroInstanceColliders(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros);

    destroyColliderManager(p_gameMgr->collMgr);
    p_gameMgr->collMgr = NULL;

    resetHerosInstanceBetweenLevel(p_gameMgr->herosMgr->heroInstance);

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
    destroyLevelIconManager(p_gameMgr->levIconMgr);
    // Already destroyed at level closure
    //destroyColliderManager(currGameManager->collMgr);
    destroyGame(p_gameMgr->currentGame);

    destroyFireBullet(p_gameMgr->fireBullet);

    free(p_gameMgr);
}//--------------------------------------------------------------------------------------------------------------------

