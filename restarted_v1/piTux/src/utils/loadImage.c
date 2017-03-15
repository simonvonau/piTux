#include "loadImage.h"
//le raspberry ne b�n�ficie pas de l'acc�l�ration donc on prend des SDL_surface au lieu des SDL_Textures

SDL_Surface* loadImage(char* path){
    SDL_Surface* pSprite = malloc(sizeof(SDL_Surface));//faudra bien faire attention de lib�rer le tas!
    pSprite=IMG_Load(path);

    if ( pSprite ){
        //printf("loadImage(%s): OK\n",path);
        return pSprite;

    }else{
        printf("Cannot find this sprite: %s \n",path);
    }
    return NULL;
}//--------------------------------------------------------------------------------------------------------------------
