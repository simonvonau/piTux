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
    int id; // Each type of Hero is referenced by it's id
    SDL_Surface **** sprites; // [State][actionId][SpriteId]
    int **spriteDuration; //[State][Actionid] Time between 2 sprites of an action
    Collider *** herosColl;//[State][Actionid] Collider (1collider per action)
    int stateSize;// currently 2 but can change if i want add more states (like invincible, fly mode, ...)
    int *actionSize;
    int **spriteSize;
    int speed, jumpSpeed;
    int timeBeforeIdle;// Time before going to idle mode
    int jumpDuration;// Time between jump start and jump max height
}Heros;

Heros *initHeros(char path[FILE_PATH_SIZE_MAX]);
void destroyHeros(Heros *currHeros);


#endif
