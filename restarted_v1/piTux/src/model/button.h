#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../utils/reporting.h"
#include "../utils/loadImage.h"
#include "../utils/arrayProcessing.h"

typedef struct{
    SDL_Surface * background;
    int isEnabled;
    int posX,posY;
    int guiHeight, guiWidth;
}Button;

Button *initButton(char *path);
void displayButton(SDL_Window *p_window, Button *currBut);
int isClicked(Button *currButt, int clicX, int clicY);
void destroyButton(Button *currButton);

#endif
