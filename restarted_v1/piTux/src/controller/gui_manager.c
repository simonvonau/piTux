#include "gui_manager.h"


void launchGuiManager(SDL_Window *p_window, GameManager *p_gameMgr){
    int nextWindowId = 1;
    int nextLevelPathSize = 512;
    char nextLevelPath[nextLevelPathSize];

    while (nextWindowId ){
        nextWindowId = openWindow(p_window, nextWindowId, nextLevelPath, nextLevelPathSize, p_gameMgr);
    }

}//------------------------------------------------------------------------------------------------------------------------

int openWindow(SDL_Window *p_window, int p_idWindow, char *p_nextLevelPath, int p_nextLevelPathSize, GameManager *p_gameMgr){
    switch(p_idWindow){
        case 1:
            return displayMainPage(p_window, p_gameMgr);
        case 2:
            return displayLevelSelectionPage(p_window, p_nextLevelPath, p_gameMgr);
        case 3:
            return displayLevelEditorPage(p_window, p_gameMgr);
        case 4:
            return displaySettingsPage(p_window, p_gameMgr);
        case 5:
            return displayCreditsPage(p_window, p_gameMgr);
        case 6:
            return displayQuitPage(p_window, p_gameMgr);
        case 7:
            return displayGamePage(p_window, p_nextLevelPath, p_nextLevelPathSize, p_gameMgr);
        default:
            return 0;

    }
}//------------------------------------------------------------------------------------------------------------------------
