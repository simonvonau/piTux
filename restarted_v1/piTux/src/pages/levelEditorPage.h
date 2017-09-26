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

void fillToolBox(SDL_Window *p_window, int p_toolType, int p_startPosX, int p_startPosY, SDL_Surface *p_cursor, int p_selectedTool, int p_cursorDecalX, int p_cursorDecalY, GameManager *p_gameMgr);
void selectTool(int p_toolType, int p_toolBoxStartX, int p_toolBoxStartY, int p_clicX, int p_clicY, int *p_selectedTool, GameManager *p_gameMgr);
int displayLevelEditorPage(SDL_Window *p_window, GameManager *p_gameMgr);

#endif
