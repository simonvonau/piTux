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
#include "../controller/levelManager.h"


typedef struct {
    Heros *heros;
    HeroInstance *heroInstance;
    char path[FILE_PATH_SIZE_MAX];
}HerosManager;


HerosManager *initHerosManager(char p_path[FILE_PATH_SIZE_MAX]);
void initHerosInstanceByManager(HerosManager *p_herosMgr);
void updateHeroBehaviourAfterCollisionDetection(HeroInstance *p_herosInstance, Heros * p_heros, ColliderManager *p_collMgr, int p_currentTime, int p_loopTime, LevelManager *p_levMgr, FireBullet *p_fireBullet);
void jumpHeros(Heros *p_heros);
void moveHeros(Heros *p_heros, int p_direction, int p_loopTime);
void displayHeros(HerosManager *p_herosMgr, SDL_Window *p_window, int p_displayedPosX, int p_displayedPosY);
void testHerosCollisions(Heros *p_heros);
void herosGatherBonus(Heros* p_heros, BonusInstance *p_bonusInstance);

void destroyHerosManager(HerosManager *p_herosManager);



#endif
