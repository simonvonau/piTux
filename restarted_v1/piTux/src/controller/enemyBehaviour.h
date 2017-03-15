#ifndef __ENEMYBEHAVIOUR_H__
#define __ENEMYBEHAVIOUR_H__
#include <stdlib.h>
#include <stdio.h>

#include "../global.h"
#include "../model/enemy.h"
#include "../model/enemyInstance.h"
#include "../controller/colliderManager.h"



void updateEnemyBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime);

void updateMrFluffyBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime);
void updateMrBombBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime);
void updateMrIceBlockBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime);

#endif
