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

Button *initButton(char *p_path);
void displayButton(SDL_Window *p_window, Button *p_button);
int isClicked(Button *p_button, int p_clicX, int p_clicY);
void destroyButton(Button *p_button);

#endif
