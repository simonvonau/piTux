#include "loadImage.h"

SDL_Surface* loadImage(char* p_path){
    SDL_Surface* pSprite ;
    pSprite = IMG_Load(p_path);

    if ( pSprite ){
        return pSprite;

    }else{
        printf("Cannot find this sprite: %s \n",p_path);
    }
    return NULL;
}//--------------------------------------------------------------------------------------------------------------------
