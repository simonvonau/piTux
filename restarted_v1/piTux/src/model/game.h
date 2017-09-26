#ifndef __GAME_H__
#define __GAME_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy


#include "../utils/reporting.h"
#include "../global.h"
#include "../utils/arrayProcessing.h"

typedef struct{
    char *allTranslationsFile;
    char *allBlocksFile;
    char *allEnemiesFile;
    char *allBonusFile;
    char *allHerosFile;
}Game;


Game* initGame(char *p_path);
void destroyGame(Game *p_game);

#endif
