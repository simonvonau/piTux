#ifndef __ENEMYBEHAVIOUR_H__
#define __ENEMYBEHAVIOUR_H__
#include <stdlib.h>
#include <stdio.h>

#include "../global.h"
#include "../model/enemy.h"
#include "../model/enemyInstance.h"
#include "../controller/colliderManager.h"



void updateEnemyBehaviourAfterCollisionDetection(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime);

void updateMrFluffyBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime);
void updateMrBombBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime);
void updateMrIceBlockBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime);

#endif
