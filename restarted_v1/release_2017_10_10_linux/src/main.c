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

    // (The music manager is declared in the global.h file)
    GameManager *currGameManager;

    // SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 || TTF_Init() == -1){
        fprintf(stdout,"Cannot initialize SDL: (%s)\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    //44100hz doesn't works on my debian pc...
    if(Mix_OpenAudio(/*44100*/22050, MIX_DEFAULT_FORMAT, 2, MUSIC_BUFFER_LENGTH) < 0){
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    currGameManager = initGameManager("./data/settings/gameSettings.txt");
    currMusicManager = initMusicManager();

    // Openning a new window
    p_window = SDL_CreateWindow("piTux", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (p_window){
        launchGuiManager(p_window, currGameManager);
    }
    else{
        fprintf(stderr,"Error: cannot open a new window %s\n",SDL_GetError());
    }

    destroyMusicManager(currMusicManager);
    destroyGameManager(currGameManager);
    SDL_DestroyWindow(p_window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    printf("Game has successfully ended! \n");

    return 1;
}
