#include "gui_manager.h"


void launchGuiManager(SDL_Window *p_window, GameManager *currGameMgr){
    int nextWindowId = 1;
    int nextLevelPathSize = 512;
    char nextLevelPath[nextLevelPathSize];

    while (nextWindowId ){
        nextWindowId = openWindow(p_window, nextWindowId, nextLevelPath, nextLevelPathSize, currGameMgr);
    }

}//------------------------------------------------------------------------------------------------------------------------

int openWindow(SDL_Window *p_window, int idWindow, char *nextLevelPath, int nextLevelPathSize, GameManager *currGameMgr){
    switch(idWindow){
        case 1:
            return displayMainPage(p_window, currGameMgr);
        case 2:
            return displayLevelSelectionPage(p_window, nextLevelPath, currGameMgr);
        case 3:
            return displayLevelEditorPage(p_window, currGameMgr);
        case 4:
            return displaySettingsPage(p_window, currGameMgr);
        case 5:
            return displayCreditsPage(p_window, currGameMgr);
        case 6:
            return displayQuitPage(p_window, currGameMgr);
        case 7:
            return displayGamePage(p_window, nextLevelPath, nextLevelPathSize, currGameMgr);
        default:
            return 0;

    }
}//------------------------------------------------------------------------------------------------------------------------
