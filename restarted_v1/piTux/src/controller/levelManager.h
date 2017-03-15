#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__
#include <stdlib.h>
#include <stdio.h>



#include "../utils/reporting.h"
#include "../utils/arrayProcessing.h"

#include "../controller/enemyBehaviour.h"
#include "../controller/colliderManager.h"

#include "../model/level.h"
#include "../model/block.h"
#include "../model/bonus.h"
#include "../model/enemy.h"
#include "../model/blockInstance.h"
#include "../model/bonusInstance.h"
#include "../model/enemyInstance.h"

typedef struct{
    Level *currLevel;
}LevelManager;

LevelManager* initLevelManager();
void loadLevelByLevelMgr(LevelManager* currLevelMgr, char *path, int pathSize);

int getBlockInstanceId(Level *lev, int clicX, int clicY);

void refreshLevelByLevelManager(Level *currLevel, ColliderManager *currCollMgr, int loopTime, Block **allBlocks, Bonus **allBonus, Enemy **allEnemies);
void displayLevelByLevelManager(Level *currLevel, SDL_Window *p_window, int isGameMode, int deplaX, int deplaY
                  , Block **allBlocks, Bonus **allBonus, Enemy **allEnemies);

void saveLevelByManager(LevelManager* currLevelManager);

void gatherBonusByCollider(LevelManager *currLevelMgr, Collider *currColl);

void destroyLevelManager(LevelManager *currLevelManager);
#endif
