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

Level *initLevel(char *path, int pathSize);
void saveLevel(Level *lev);

void addBlockInstanceToLevel(Level *lev, int idBlock, int posX, int posY, Collider *coll);
void addBonusInstanceToLevel(Level *lev, int idBonus, int posX, int posY, Collider *coll);
void addEnemyInstanceToLevel(Level *lev, int idEnemy, int posX, int posY, Collider **coll, int collSize);

void removeBlockInstanceToLevel(Level *lev, int blockIndex);
void removeBonusInstanceToLevel(Level *lev, int bonusIndex);
void removeEnemyInstanceToLevel(Level *lev, int enemyIndex);

int checkIfBlockInstanceExistHere(Level *lev, int posX, int posY);
int checkIfBonusInstanceExistHere(Level *lev, int posX, int posY);
int checkIfEnemyInstanceExistHere(Level *lev, int posX, int posY);

void addBonusToBlock(Level *lev, int idBlockInstance, int idBonus);

void destroyLevel(Level *currLevel);
#endif
