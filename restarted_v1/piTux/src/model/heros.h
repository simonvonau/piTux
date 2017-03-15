#ifndef __HEROS_H__
#define __HEROS_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../model/collider.h"
#include "../utils/reporting.h"
#include "../global.h"
#include "../utils/arrayProcessing.h"


typedef struct{
    SDL_Surface **** sprites; // [currState][currActionId][currSpriteId]
    int **spriteDuration; // Time between 2 sprites of an action
    Collider *** herosColl;//[currState][currActionid]
    int stateSize;// currently 2 but can change if i want add more states (like invicible, fly mode, ...)
    int *actionSize;
    int **spriteSize;
    int currState;// Is tux big(1) or small(0)
    int currAction;
    int currSprite;
    int isDead;
    int speed, jumpSpeed;
    int lifesLeft;
    int nbCoins;
    int lastMovementTime;// Last time when tux move
    int timeBeforeIdle;// Time before going to idle mode
    int isTouchingGround;
    int jumpKeyPressed;// (0 or 1)When player press the jump key
    int jumpStartTime;// Time at jump start
    int jumpDuration;// Time between jump start and jump max height
    int posX, posY, lastX, lastY;
    int currentTime;
}Heros;

Heros *initHeros(char path[FILE_PATH_SIZE_MAX]);
void refreshHeros(Heros * p_heros, int loopTime);
void dieHeros(Heros * currHeros);
void destroyHeros(Heros *currHeros);


#endif
