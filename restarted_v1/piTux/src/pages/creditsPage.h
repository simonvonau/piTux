#ifndef __CREDITSPAGE_H__
#define __CREDITSPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include "../utils/reporting.h"
#include "../utils/loadImage.h"

#include "../controller/gameManager.h"
#include "../pages/textLayout.h"
#include "../global.h"


int displayCreditsPage(SDL_Window *p_window, GameManager *currGameMgr);

#endif
