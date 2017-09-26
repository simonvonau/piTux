#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__
#include <stdlib.h>
#include <stdio.h>

#include "../utils/reporting.h"
#include "../global.h"

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


GameManager* initGameManager(char *p_path);

void loadLevelByGameMgr(GameManager *p_gameMgr, char *p_path, int p_pathSize);
void addElementToLevelByGameMgr(GameManager *p_gameMgr, int p_elemType, int p_elemId, int p_posX, int p_posY);
void removeElementFromLevel(Level *p_lev, int p_clicX, int p_clicY);
void refreshGameByGameManager(GameManager *p_gameMgr, int p_currentTime, int p_loopTime, int p_screenWidth, int p_screenHeight, int p_deplaX, int p_deplaY);
void cleanLevelMemory(GameManager *p_gameMgr);

void destroyLevelByGameManager(GameManager *p_gameMgr);
void destroyGameManager(GameManager *p_gameMgr);



#endif
