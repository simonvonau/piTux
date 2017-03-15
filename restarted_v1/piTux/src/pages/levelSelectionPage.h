#ifndef __LEVELSELECTIONPAGE_H__
#define __LEVELSELECTIONPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"

#include "../controller/gameManager.h"

#include "../model/levelIcon.h"
#include "../pages/textLayout.h"

int chooseLevel(LevelIcon **allLevels, int nbLevels, int userChoice, char *keyPressed);
int displayLevelSelectionPage(SDL_Window *p_window, char *nextLevelPath, GameManager *currGameMgr);

#endif
