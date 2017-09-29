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
#include "controller/musicManager.h"



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
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, MUSIC_BUFFER_LENGTH) < 0){
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    currGameManager = initGameManager("./data/settings/gameSettings.txt");
    currMusicManager = initMusicManager(MUSIC_BUFFER_LENGTH, 10);


    // Play a song
    /*Mix_AllocateChannels(32); //Allouer 32 canaux
    Mix_Volume(1, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 1
    Mix_Chunk *son;
    son = Mix_LoadWAV("./data/sounds/jump.wav");
    if(son == NULL){
        printf("errors : %s", Mix_GetError());
    }
    Mix_VolumeChunk(son, MIX_MAX_VOLUME);
    Mix_PlayChannel(1, son, 0);
    if(Mix_PlayChannel( 0, son, 0 ) == -1){
        printf("errors : %s", Mix_GetError());
    }
    SDL_Delay(2000);
    Mix_FreeChunk(son);
    fgetc(stdin);*/

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
