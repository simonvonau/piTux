#include "herosManager.h"





HerosManager *initHerosManager(char path[FILE_PATH_SIZE_MAX]){
// Init a new HerosManager
    HerosManager *res = malloc(sizeof(HerosManager));
    if(res == NULL){
        reportErreur("initHerosManager():error malloc()");
    }
    res->heros = initHeros(path);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void refreshHerosManager(Heros *currHeros, int timeLoop){
    refreshHeros(currHeros, timeLoop);
}//------------------------------------------------------------------------------------------------------------------------

void jumpHeros(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void moveHeros(Heros *currHeros, int direction, int timeLoop){

}//------------------------------------------------------------------------------------------------------------------------

void displayHeros(Heros *currHeros, SDL_Window *p_window){
    SDL_Rect objectPos = { currHeros->posX, 90/*currHeros->posX + currHeros->herosColl[0][0]->width*/
                            , currHeros->posY, 200/*currHeros->posY + currHeros->herosColl[0][0]->height*/};

    SDL_BlitSurface(currHeros->sprites[currHeros->currState][currHeros->currAction][currHeros->currSprite],
                             NULL, SDL_GetWindowSurface(p_window), &objectPos);
}//------------------------------------------------------------------------------------------------------------------------

void testHerosCollisions(Heros *currHeros){

}//------------------------------------------------------------------------------------------------------------------------

void herosGatherBonus(Heros* currHeros, BonusInstance *currBonusInstance){

}//------------------------------------------------------------------------------------------------------------------------

void destroyHerosManager(HerosManager *currHerosManager){
// Free HerosManager memory
    destroyHeros(currHerosManager->heros);
    free(currHerosManager);
}//------------------------------------------------------------------------------------------------------------------------

