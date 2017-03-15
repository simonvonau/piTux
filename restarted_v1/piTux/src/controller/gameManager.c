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

    //Destroying previous level if exists
    //if(currGameMgr->levelManager->currLevel != NULL){destroyLevel(currGameMgr->levelManager->currLevel);}
    loadLevelByLevelMgr(currGameMgr->levelManager, path, pathSize);

    // Destroying previous collisions if exists
    //if(currGameMgr->collMgr != NULL){destroyColliderManager(currGameMgr->collMgr);}
    currGameMgr->collMgr = initColliderManager();

    // Put Collider on each BlockInstance
    for( i = 0; i < currGameMgr->levelManager->currLevel->blockInstancesSize; i++){
        currGameMgr->levelManager->currLevel->blockInstances[i]->coll = colliderDeepCopyByColliderManager(
                currGameMgr->collMgr
                , currGameMgr->allBlocks[ currGameMgr->levelManager->currLevel->blockInstances[i]->idBlock ]->refColl);
        // Update BlockInstance Collider
        currGameMgr->levelManager->currLevel->blockInstances[i]->coll->isEnabled = 1;
        currGameMgr->levelManager->currLevel->blockInstances[i]->coll->posX = currGameMgr->levelManager->currLevel->blockInstances[i]->posX;
        currGameMgr->levelManager->currLevel->blockInstances[i]->coll->posY = currGameMgr->levelManager->currLevel->blockInstances[i]->posY;
    }
    // Put Collider on each BonusInstance
    for( i = 0; i < currGameMgr->levelManager->currLevel->bonusInstancesSize; i++){
        currGameMgr->levelManager->currLevel->bonusInstances[i]->coll = colliderDeepCopyByColliderManager(
                currGameMgr->collMgr
                ,currGameMgr->allBonus[ currGameMgr->levelManager->currLevel->bonusInstances[i]->idBonus ]->refColl);
        // Update BonusInstance Collider
        currGameMgr->levelManager->currLevel->bonusInstances[i]->coll->isEnabled = 1;
        currGameMgr->levelManager->currLevel->bonusInstances[i]->coll->posX = currGameMgr->levelManager->currLevel->bonusInstances[i]->posX;
        currGameMgr->levelManager->currLevel->bonusInstances[i]->coll->posY = currGameMgr->levelManager->currLevel->bonusInstances[i]->posY;
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
            currGameMgr->levelManager->currLevel->enemyInstances[i]->coll[j]->posX = currGameMgr->levelManager->currLevel->enemyInstances[i]->posX;
            currGameMgr->levelManager->currLevel->enemyInstances[i]->coll[j]->posY = currGameMgr->levelManager->currLevel->enemyInstances[i]->posY;
            if (j == currGameMgr->levelManager->currLevel->enemyInstances[i]->currentActionId){
                currGameMgr->levelManager->currLevel->enemyInstances[i]->coll[j]->isEnabled = 1;
            }else{
                currGameMgr->levelManager->currLevel->enemyInstances[i]->coll[j]->isEnabled = 0;
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

void refreshGameByGameManager(GameManager *currGameMgr, int loopTime, int screenWidth, int screenHeight, int deplaX, int deplaY){
// Refresh game
    // Set a margin out of the screen
    int margin = 200;
    int i;

    refreshHeros(currGameMgr->herosMgr->heros, loopTime);
    updateHeroBehaviour(currGameMgr->herosMgr->heros, currGameMgr->collMgr, loopTime);

    refreshLevelByLevelManager(currGameMgr->levelManager->currLevel, currGameMgr->collMgr, loopTime
                    ,currGameMgr->allBlocks, currGameMgr->allBonus, currGameMgr->allEnemies);

    updateCollisions(currGameMgr->collMgr
                    , -deplaX - margin
                    , -deplaX + margin + screenWidth
                    , deplaY - margin
                    , deplaY + margin + screenHeight);

    // Problem: Multiples calls of updateEnemyBehaviour() => spends process time

    for(i = 0; i < currGameMgr->levelManager->currLevel->enemyInstancesSize; i++){
        updateEnemyBehaviour(currGameMgr->levelManager->currLevel->enemyInstances[i]
                             ,*currGameMgr->allEnemies[currGameMgr->levelManager->currLevel->enemyInstances[i]->idEnemy]
                             ,currGameMgr->collMgr
                             ,loopTime);
    }


}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelByGameManager(GameManager *currGameManager){
// Free Level,Collisions et Colliders memory
    destroyColliderManager(currGameManager->collMgr);
    currGameManager->collMgr = NULL;

    destroyLevel(currGameManager->levelManager->currLevel);
    currGameManager->levelManager->currLevel = NULL;

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

