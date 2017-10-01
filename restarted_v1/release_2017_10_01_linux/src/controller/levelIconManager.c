#include "levelIconManager.h"


LevelIconManager *initLevelIconManager(char *p_path){
// Initialize the levelIconManager
    LevelIconManager *res = malloc(sizeof(LevelIconManager));
    if(res == NULL){
        reportErreur("Error initTranslationManager malloc()");
    }

    res->allLevIcons = initLevelIcons(p_path, &res->allLevIconsSize);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void achieveLevel(LevelIconManager *p_levIconMgr, int p_idLevel){
// Set a level to the state "clear"
    int linkedLevel;
    if(p_idLevel >= 0 && p_idLevel < p_levIconMgr->allLevIconsSize){
        p_levIconMgr->allLevIcons[p_idLevel]->isCleared = 1;

        // Unlock the 2 levels adjacent
        linkedLevel = p_levIconMgr->allLevIcons[p_idLevel]->linkedLevel1;
        if(linkedLevel >= 0 && linkedLevel < p_levIconMgr->allLevIconsSize){
            p_levIconMgr->allLevIcons[linkedLevel]->isAvailable = 1;
        }
        linkedLevel = p_levIconMgr->allLevIcons[p_idLevel]->linkedLevel2;
        if(linkedLevel >= 0 && linkedLevel < p_levIconMgr->allLevIconsSize){
            p_levIconMgr->allLevIcons[linkedLevel]->isAvailable = 1;
        }
    }else{
        reportErreur("achieveLevel index out of range");
    }
}//------------------------------------------------------------------------------------------------------------------------

void resetLevelAchievements(LevelIconManager *p_levIconMgr){
// Set all the level to the "unclear" and "non-available" state
    int i;
    for(i = 0; i < p_levIconMgr->allLevIconsSize; i++){
        p_levIconMgr->allLevIcons[i]->isCleared = 0;
        p_levIconMgr->allLevIcons[i]->isAvailable = 0;
    }
    // Unlock the first level
    if(p_levIconMgr->allLevIconsSize > 0){
        p_levIconMgr->allLevIcons[0]->isAvailable = 0;
    }
}//------------------------------------------------------------------------------------------------------------------------

void destroyLevelIconManager(LevelIconManager *p_levIconMgr){
// Destroy the levelIconManager
    int i;
    for(i = 0; i < p_levIconMgr->allLevIconsSize; i++){
        destroyLevelIcon(p_levIconMgr->allLevIcons[i]);
    }
    free(p_levIconMgr->allLevIcons);
    free(p_levIconMgr);
}//------------------------------------------------------------------------------------------------------------------------

