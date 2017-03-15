#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__
#include <stdlib.h>
#include <stdio.h>

#include "../utils/reporting.h"

#include "../controller/enemyBehaviour.h"
#include "../controller/translationManager.h"
#include "../controller/herosManager.h"
#include "../controller/levelManager.h"
#include "../controller/colliderManager.h"
#include "../model/game.h"
#include "../model/block.h"
#include "../model/bonus.h"

typedef struct{
    Game *currentGame;
    TranslationManager *translaManager;
    ColliderManager *collMgr;
    LevelManager *levelManager;
    HerosManager *herosMgr;
    Block **allBlocks;
    int allBlocksSize;
    Bonus **allBonus;
    int allBonusSize;
    Enemy **allEnemies;
    int allEnemiesSize;
}GameManager;


GameManager* initGameManager(char *path);

void loadLevelByGameMgr(GameManager *currGameMgr, char *path, int pathSize);
void addElementToLevelByGameMgr(GameManager *currGameMgr, int elemType, int elemId, int posX, int posY);
void removeElementFromLevel(Level *lev, int clicX, int clicY);
void refreshGameByGameManager(GameManager *currGameMgr, int loopTime, int screenWidth, int screenHeight, int deplaX, int deplaY);

void destroyLevelByGameManager(GameManager *currGameManager);
void destroyGameManager(GameManager *currGameManager);



#endif
