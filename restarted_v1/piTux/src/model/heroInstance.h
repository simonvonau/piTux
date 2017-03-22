#ifndef __HEROINSTANCE_H__
#define __HEROINSTANCE_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include "../model/collider.h"
#include "../model/heros.h"
#include "../utils/reporting.h"
#include "../global.h"
#include "../utils/arrayProcessing.h"


typedef struct{
    int idHeros;
    Collider *** herosColl;//[currState][currActionid]
    int currState;// Is tux big(1) or small(0)
    int currAction;
    int currSprite;
    int isDead;
    int lifesLeft;
    int nbCoins;
    int lastMovementTime;// Last time when tux move
    int isTouchingGround;
    int jumpKeyPressed;// (0 or 1)When player press the jump key
    int jumpStartTime;// Time at jump start
    int jumpDuration;// Time between jump start and jump max height
    int posX, posY, lastPosX, lastPosY;
    float movementProgressX, movementProgressY; // // Store a movement <1 pixel => to avoid being stuck if FPS is too fast
    int currentTime;
}HeroInstance;


HeroInstance * initHeroInstance();
void refreshHeroInstance(HeroInstance * p_heroInstance, Heros *p_heros, int loopTime);
void destroyHeroInstanceColliders(HeroInstance *p_heroInstance, Heros *p_heros);
void destroyHeroInstance(HeroInstance *p_heroInstance);




#endif
