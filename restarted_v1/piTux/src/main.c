#include <stdio.h>
#include <stdlib.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mixer.h>

#include "global.h"
#include "controller/gui_manager.h"
#include "controller/gameManager.h"




int main(int argc, char *argv[])
{
    SDL_Window* p_window = NULL;

    // Init game data
    GameManager *CurrGameManager = initGameManager("./data/settings/gameSettings.txt");
    //CurrMusicManager = initMusicManager(MUSIC_BUFFER_LENGTH, 10);

    // SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 || TTF_Init() == -1){
        fprintf(stdout,"Cannot initialize SDL: (%s)\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    /*if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, MUSIC_BUFFER_LENGTH) < 0){
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }*/

    // Openning a new window
    p_window = SDL_CreateWindow("piTux", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (p_window){
        launchGuiManager(p_window, CurrGameManager);
    }
    else{
        fprintf(stderr,"Error: cannot open a new window %s\n",SDL_GetError());
    }

    //destroyMusicManager(CurrMusicManager);
    destroyGameManager(CurrGameManager);
    SDL_DestroyWindow(p_window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    printf("Game has successfully ended! \n");

    return 1;
}
