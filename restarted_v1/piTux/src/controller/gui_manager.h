#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "gameManager.h"
#include "../utils/reporting.h"
#include "../pages/mainPage.h"
#include "../pages/creditsPage.h"
#include "../pages/gamePage.h"
#include "../pages/levelEditorPage.h"
#include "../pages/levelSelectionPage.h"
#include "../pages/quitPage.h"
#include "../pages/settingsPage.h"


void launchGuiManager(SDL_Window *p_window, GameManager *currGameMgr);
int openWindow(SDL_Window *p_window, int idWindow, char *nextLevelPath, int nextLevelPathSize, GameManager *currGameMgr);

#endif
