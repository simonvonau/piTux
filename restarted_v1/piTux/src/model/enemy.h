#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"
#include "../model/collider.h"
#include "../utils/arrayProcessing.h"


typedef struct
{
    int idEnemy;
    SDL_Surface *** sprites; //[actionId][SpriteId]
    int spritesSize1;
    int *spritesSize2;
    int *spriteDuration; // All actions life-time
    int *speed; // All speed sorted by actionId
    Collider **actionColl;// All the colliders (sorted by actionId)
    int enemyType; // cf TAG from global.h
    void *specialBehaviour; // To define his own behaviour
} Enemy;

Enemy **initEnemyArray(char *p_path,int *p_resSize);
Enemy *initEnemy(char *p_path);
void enemyDie(Enemy *p_enemy);
void destroyEnemy(Enemy *p_enemy);


#endif
