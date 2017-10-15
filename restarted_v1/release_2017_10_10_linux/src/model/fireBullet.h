#ifndef __FIREBULLET_H__
#define __FIREBULLET_H__
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
    SDL_Surface ** sprites;
    int spritesSize1; // sprites array size
    int spriteDuration; // All actions life time
    Collider *refColl; // Reference collider for FireBulletInstance creation
    int speedX, speedY;
    int maxLifeTime;
    int maxJumpHeight; // Max height of a bullet jump
}FireBullet;


FireBullet * initFireBullet( char *p_path);
void destroyFireBullet(FireBullet *p_fireBullet);


#endif
