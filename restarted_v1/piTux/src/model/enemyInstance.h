#ifndef __ENEMYINSTANCE_H__
#define __ENEMYINSTANCE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../model/enemy.h"
#include "../model/collider.h"


typedef struct{
    int idEnemy;
    int currentTime;
    int currentActionId;
    int currentSpriteId;
    float movementProgressX; // Store a movement <1 pixel => to avoid being stuck if FPS is too fast
    float movementProgressY;
    int posX, posY;
    int isDead;
    int direction;
    int isFalling;
    Collider ** coll;
    int collSize;
}EnemyInstance;


EnemyInstance * initEnemyInstance(int idEnemy, int posX, int posY, Collider ** currColl, int collSize);
void refreshEnemyInstance(EnemyInstance *currEnemyInstance, int loopTime, Enemy currEnemy);
void displayEnemyInstance(EnemyInstance *currEnemyInstance, Enemy currEnemy, SDL_Window *p_window, int deplaX, int deplaY);
void destroyEnemyInstance(EnemyInstance *currEnemyInstance);


#endif
