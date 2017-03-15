#ifndef __HELPPAGE_H__
#define __HELPPAGE_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../../global.h"
#include "../../utils/reporting.h"
#include "../../utils/loadImage.h"

#include "../../controller/gameManager.h"
#include "../../pages/textLayout.h"
#include "../../model/button.h"

//NB: Sub pages are not managed by gui_manager and can be called by another page
int displayHelpSubPage(SDL_Window *p_window, GameManager *currGameMgr);

#endif
