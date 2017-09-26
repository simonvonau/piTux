#ifndef __TEXTLAYOUT_H__
#define __TEXTLAYOUT_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/arrayProcessing.h"

void setTextLayout(SDL_Window* p_window, char *p_text,int p_textSize,TTF_Font* p_font,SDL_Color p_textColor,SDL_Rect p_textPos);
char *setTimeLayout(int p_time, int p_wishedSize);

#endif
