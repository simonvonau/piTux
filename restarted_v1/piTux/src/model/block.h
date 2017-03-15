#ifndef __BLOCK_H__
#define __BLOCK_H__
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
    SDL_Surface *** sprites;
    int spritesSize1; // sprites size
    int *spritesSize2;// List of sprites[*] sizes
    int *spriteDuration; // All actions life time
    char blockType;// cf global.h
    Collider *refColl; // A non-registered Collider

}Block;

Block ** initBlockArray(char *path, int *resSize);
Block * initBlock( char *path);
void destroyBlock(Block *currBlock);


#endif
