#include "herosManager.h"





HerosManager *initHerosManager(char path[FILE_PATH_SIZE_MAX]){
// Init a new HerosManager
    int i;
    HerosManager *res = malloc(sizeof(HerosManager));
    strncpy(res->path, path, FILE_PATH_SIZE_MAX);

    if(res == NULL){
        reportErreur("initHerosManager():error malloc()");
    }
    initHerosByManager(res);

    //
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void initHerosByManager(HerosManager *herosMgr){
// Initialize a new hero, have to call 1 time per level
    herosMgr->heros = initHeros(herosMgr->path);
    herosMgr->heroInstance = initHeroInstance();

}//------------------------------------------------------------------------------------------------------------------------

void updateHeroBehaviour(Heros *currHeros, ColliderManager *collMgr, int loopTime){
// Update the hero behaviour (especially when he is touching something)

}//------------------------------------------------------------------------------------------------------------------------

void jumpHeros(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void moveHeros(Heros *currHeros, int direction, int timeLoop){

}//------------------------------------------------------------------------------------------------------------------------

void displayHeros(Heros *currHeros, SDL_Window *p_window){
// Display the hero
    /*SDL_Rect objectPos = { currHeros->posX, currHeros->posX + currHeros->herosColl[currHeros->currState][currHeros->currAction]->width
                            , currHeros->posY, currHeros->posY + currHeros->herosColl[currHeros->currState][currHeros->currAction]->height};

    SDL_BlitSurface(currHeros->sprites[currHeros->currState][currHeros->currAction][currHeros->currSprite],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);*/
}//------------------------------------------------------------------------------------------------------------------------

void testHerosCollisions(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void herosGatherBonus(Heros* currHeros, BonusInstance *currBonusInstance){

}//------------------------------------------------------------------------------------------------------------------------

void destroyHerosManager(HerosManager *currHerosManager){
// Free HerosManager memory
    destroyHeros(currHerosManager->heros);
    destroyHeroInstance(currHerosManager->heroInstance);
    free(currHerosManager);
}//------------------------------------------------------------------------------------------------------------------------

