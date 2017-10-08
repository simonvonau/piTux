#ifndef __GAMEPAGE_H__
#define __GAMEPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"

#include "../pages/textLayout.h"
#include "../pages/sub_pages/break_page.h"

#include "../controller/levelManager.h"
#include "../controller/gameManager.h"

#include "../model/enemy.h"
#include "../model/heros.h"
#include "../model/block.h"
#include "../model/level.h"
#include "../model/button.h"


int displayGamePage(SDL_Window *p_window, char *p_levelPath, int p_levelPathSize, GameManager *p_gameMgr);

#endif
