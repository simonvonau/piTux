#ifndef __LEVELEDITORPAGE_H__
#define __LEVELEDITORPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#include "../global.h"
#include "../utils/reporting.h"
#include "../utils/loadImage.h"

#include "../pages/textLayout.h"
#include "../pages/sub_pages/help_page.h"

#include "../controller/levelManager.h"
#include "../controller/gameManager.h"

#include "../model/enemy.h"
#include "../model/heros.h"
#include "../model/block.h"
#include "../model/level.h"
#include "../model/button.h"

void fillToolBox(SDL_Window *p_window, int toolType, int startPosX, int startPosY, SDL_Surface *cursor, int selectedTool
                , int cursorDecalX, int cursorDecalY, GameManager *currGameMgr);
void selectTool(int toolType, int toolBoxStartX, int toolBoxStartY, int clicX, int clicY, int *selectedTool, GameManager *currGameMgr);
int displayLevelEditorPage(SDL_Window *p_window, GameManager *currGameMgr);

#endif
