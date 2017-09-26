#ifndef __LEVELICON_H__
#define __LEVELICON_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include "../utils/reporting.h"
#include "../utils/arrayProcessing.h"

typedef struct{
    int posX,posY;
    int isCleared;
    int isAvailable;
    int linkedLevel1,linkedLevel2;
    char path[512];
}LevelIcon;

LevelIcon **initLevelIcons(char *p_path, int *p_resSize);
LevelIcon *initLevelIcon(int p_lineSizeMax, int p_startIndex, char ** p_buff);
void destroyLevelIcon(LevelIcon *p_levelIcon);

#endif
