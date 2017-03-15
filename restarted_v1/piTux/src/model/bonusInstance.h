#ifndef __BONUSINSTANCE_H__
#define __BONUSINSTANCE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../utils/reporting.h"
#include "../model/collider.h"
#include "../model/bonus.h"

typedef struct{
    int idBonus;
    int currentTime;
    int currentActionId;
    int currentSpriteId;
    int posX, posY;
    Collider *coll;
}BonusInstance;


BonusInstance * initBonusInstance(int idBonus, int posX, int posY, Collider *coll);
void refreshBonusInstance(BonusInstance *currBonusInstance, int loopTime, Bonus currBonus);
void destroyBonusInstance(BonusInstance *currBonusInstance);


#endif
