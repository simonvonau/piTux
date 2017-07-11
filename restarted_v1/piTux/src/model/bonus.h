#ifndef __BONUS_H__
#define __BONUS_H__
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

typedef struct{
    SDL_Surface *** sprites;
    int spritesSize1; // sprites array size
    int *spritesSize2;// List of sprites[*] sizes
    int *spriteDuration; // All actions life time
    int bonusType;// cf global.h
    Collider *refColl;
    int speedX, speedY;
}Bonus;

Bonus ** initBonusArray(char *path, int *resSize);
Bonus * initBonus( char *path);
void destroyBonus(Bonus *currBonus);


#endif
