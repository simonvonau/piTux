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
    int isTouchingGround;// If the heros is on the ground (or on an ennemy)
    int jumpKeyPressed;// (0 or 1)When player press the jump key
    int leftKeyPressed;// (0 or 1)
    int rightKeyPressed;// (0 or 1)
    int jumpStartTime;// Time at jump start
    int jumpDuration;// Time after jump starts
    char lastDirection;// 'r' or 'l'
    int posX, posY, lastPosX, lastPosY;
    float movementProgressX, movementProgressY; // Store a movement <1 pixel => to avoid being stuck if FPS is too fast
    int currentTime;
    int godModeDuration; // The god mode allow tux to touch enemies without troubles
}HeroInstance;

HeroInstance * initHeroInstance();

void changeHerosState(HeroInstance * p_heroInstance,int p_newState, int p_newAction, int p_newSprite);
void changeHeroAction(HeroInstance * p_heroInstance, int p_newAction, int p_newSprite);

// Refreshing the heros
void refreshHeroInstance(HeroInstance * p_heroInstance, Heros *p_heros, int currentTime, int loopTime);
void applyGravity(HeroInstance *p_heroInstance, int p_loopTime);
void movingRight(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime);
void movingLeft(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime);
void jump(HeroInstance *p_heroInstance, Heros *p_heros, int p_loopTime);
void land(HeroInstance *p_heroInstance);
void updateCurrentSprite(HeroInstance *p_heroInstance, Heros *p_heros);

void destroyHeroInstanceColliders(HeroInstance *p_heroInstance, Heros *p_heros);
void destroyHeroInstance(HeroInstance *p_heroInstance);




#endif
