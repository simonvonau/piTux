#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/reporting.h"
#include "../utils/arrayProcessing.h"


#include "../model/block.h"
#include "../model/bonus.h"
#include "../model/enemy.h"
#include "../model/blockInstance.h"
#include "../model/bonusInstance.h"
#include "../model/enemyInstance.h"

typedef struct{
    char path[512];
    int herosStartX,herosStartY;
    int finishPosX;
    int timeLimit; // time laps to end this level
    int timeLeft;
    EnemyInstance ** enemyInstances;
    int enemyInstancesSize;
    BlockInstance ** blockInstances;
    int blockInstancesSize;
    BonusInstance ** bonusInstances;
    int bonusInstancesSize;
}Level;

Level *initLevel(char *p_path, int p_pathSize);
void saveLevel(Level *p_level);

void addBlockInstanceToLevel(Level *p_lev, int p_idBlock, int p_posX, int p_posY, Collider *p_coll);
void addBonusInstanceToLevel(Level *p_lev, int p_idBonus, int p_posX, int p_posY, Collider *p_coll);
void addEnemyInstanceToLevel(Level *p_lev, int p_idEnemy, int p_posX, int p_posY, Collider **p_coll, int p_collSize);

void removeBlockInstanceToLevel(Level *p_lev, int p_blockIndex);
void removeBonusInstanceToLevel(Level *p_lev, int p_bonusIndex);
void removeEnemyInstanceToLevel(Level *p_lev, int p_enemyIndex);

int checkIfBlockInstanceExistHere(Level *p_lev, int p_posX, int p_posY);
int checkIfBonusInstanceExistHere(Level *p_lev, int p_posX, int p_posY);
int checkIfEnemyInstanceExistHere(Level *p_lev, int p_posX, int p_posY);

void addBonusToBlock(Level *p_lev, int p_idBlockInstance, int p_idBonus);

void destroyLevel(Level *p_level);
#endif
