#ifndef __MAINPAGE_H__
#define __MAINPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"

#include "../controller/gameManager.h"

#include "../pages/textLayout.h"

int displayMainPage(SDL_Window *p_window, GameManager *p_gameMgr);

#endif
