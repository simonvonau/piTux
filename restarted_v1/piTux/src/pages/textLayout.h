#ifndef __TEXTLAYOUT_H__
#define __TEXTLAYOUT_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/arrayProcessing.h"

void setTextLayout(SDL_Window* pWindow,char *text,int textSize,TTF_Font* font,SDL_Color textColor,SDL_Rect textPos);
char *setTimeLayout(int time, int wishedSize);

#endif
