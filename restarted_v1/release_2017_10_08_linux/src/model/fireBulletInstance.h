#ifndef __FIREBULLETINSTANCE_H__
#define __FIREBULLETINSTANCE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"
#include "../model/collider.h"
#include "../model/fireBullet.h"
#include "../utils/arrayProcessing.h"


typedef struct{
    int posX, posY;
    float movementX, movementY;
    int directionX, directionY;
    int lastMinY;
    Collider *coll;
    int currentSpriteId;
    int lifeTimeLeft;
    int currentTime;
}FireBulletInstance;

FireBulletInstance * initFireBulletInstance(int p_posX, int p_posY, int p_directionX, int p_directionY, Collider *p_coll, int p_lifeTimeLeft);
void refreshFireBulletInstance(FireBulletInstance *p_fbi, FireBullet p_fireBullet, int p_loopTime);
void destroyFireBulletInstance(FireBulletInstance *p_fireBulletInstance);


#endif
