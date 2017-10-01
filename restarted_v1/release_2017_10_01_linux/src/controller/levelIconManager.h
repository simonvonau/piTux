#ifndef __LEVELICONMANAGER_H__
#define __LEVELICONMANAGER_H__
#include <stdlib.h>
#include <stdio.h>


#include "../utils/reporting.h"

#include "../model/levelIcon.h"



typedef struct{
    LevelIcon ** allLevIcons;
    int allLevIconsSize;
}LevelIconManager;

LevelIconManager *initLevelIconManager(char *p_path);
void achieveLevel(LevelIconManager *p_levIconMgr, int p_idLevel);
void resetLevelAchievements(LevelIconManager *p_levIconMgr);
void destroyLevelIconManager(LevelIconManager *p_levIconMgr);




#endif
