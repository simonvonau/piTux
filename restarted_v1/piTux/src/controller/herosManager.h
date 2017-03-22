#ifndef __HEROSMANAGER_H__
#define __HEROSMANAGER_H__
#include <stdlib.h>
#include <stdio.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../utils/reporting.h"
#include "../model/heros.h"
#include "../model/heroInstance.h"
#include "../model/bonusInstance.h"
#include "../controller/colliderManager.h"


typedef struct{
    Heros *heros;
    HeroInstance *heroInstance;
    char path[FILE_PATH_SIZE_MAX];
}HerosManager;


HerosManager *initHerosManager(char path[FILE_PATH_SIZE_MAX]);

void updateHeroBehaviour(HeroInstance *currHeroInstance, ColliderManager *collMgr, int loopTime);
void jumpHeros(Heros *currHeros);
void moveHeros(Heros *currHeros, int direction, int timeLoop);
void displayHeros(HerosManager *currHerosMgr, SDL_Window *p_window);
void testHerosCollisions(Heros *currHeros);
void herosGatherBonus(Heros* currHeros, BonusInstance *currBonusInstance);

void destroyHerosManager(HerosManager *currHerosManager);



#endif
