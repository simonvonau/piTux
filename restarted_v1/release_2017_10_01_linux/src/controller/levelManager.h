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
void loadLevelByLevelMgr(LevelManager* p_levMgr, char *p_path, int p_pathSize);

int getBlockInstanceId(Level *p_lev, int p_clicX, int p_clicY);

void addBulletInstanceFromLevelMgr(ColliderManager *p_collMgr, LevelManager* p_levMgr, int p_posX, int p_posY, FireBullet *p_fb, int p_directionX);

void refreshLevelByLevelManager(Level *p_lev, ColliderManager *p_collMgr, int p_loopTime, Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies, FireBullet *p_fireBullet, int p_leftLimit, int p_rightLimit);
void displayLevelByLevelManager(Level *p_lev, SDL_Window *p_window, int p_isGameMode, int p_deplaX, int p_deplaY
                  , Block **p_allBlocks, Bonus **p_allBonus, Enemy **p_allEnemies, FireBullet *p_fireBullet);


void updateLevelAfterCollisionsDetection(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, int p_minX, int p_maxX, Bonus** p_allBonus, Enemy **p_allEnemies, int p_allBonusSize);
void updateEnemyAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_loopTime, Enemy ** p_allEnemies, int p_minX, int p_maxX);
void updateBlocksAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, Bonus** p_allBonus, int p_allBonusSize, int p_minX, int p_maxX);
void updateBonusAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, int p_minX, int p_maxX);
void updateFireBulletAfterColliding(LevelManager* p_levMgr, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime);

void saveLevelByManager(LevelManager* p_levMgr);
void destroyLevelManager(LevelManager *p_levMgr);
#endif
