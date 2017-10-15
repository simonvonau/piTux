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
    float movementX, movementY; // A decimal position is the FPS is too fast
    int posX, posY;
    int direction; // The egg direction
    Collider *coll;
    int wasGathered;// 0 or 1 if the bonus was already gathered
    int lifeTime;// Nb of milliseconds
}BonusInstance;


BonusInstance * initBonusInstance(int p_idBonus, int p_posX, int p_posY, Collider *p_coll);
void refreshBonusInstance(BonusInstance *p_bonusInstance, int p_loopTime, Bonus p_bonus);
void destroyBonusInstance(BonusInstance *p_bonusInstance);


#endif
