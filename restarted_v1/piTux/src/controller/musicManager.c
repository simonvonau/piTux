#include "musicManager.h"



MusicManager *initMusicManager(int bufferLen, int channelsQty){
// Create a new MusicManager
    MusicManager *res = malloc(sizeof(MusicManager));
    if (res == NULL){
        reportErreur("initMusicManager(): malloc() error");
    }
    Mix_AllocateChannels(channelsQty);
    Mix_Volume(0, MIX_MAX_VOLUME/2);
    res->bufferLength = bufferLen;
    res->currMusic = NULL;

    // Loading all sounds
    res->allSoundSize = 1;
    res->allSound = malloc( res->allSoundSize * sizeof(Mix_Chunk *));
    if( res->allSound == NULL ){
        reportErreur("initMusicManager(): malloc() error2");
    }
    res->allSound[0] = Mix_LoadWAV("data/sounds/jump.wav");
    if(res->allSound[0] == NULL){
        printf("errooooooor: %s\n",Mix_GetError());
    }

    //Mix_VolumeChunk(res->allSound[0], MIX_MAX_VOLUME);
    //*** test:Mix_PlayChannel(1, currMusicMgr->allSound[0], 1);
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void loadMusic(MusicManager *currMusicMgr, char* path){
// Load an other music from pathfile
    if (currMusicMgr->currMusic != NULL){
        Mix_FreeMusic(currMusicMgr->currMusic);
    }
    if(!(currMusicMgr->currMusic=Mix_LoadMUS(path))){
        reportErreur("loadMusic(): cannot find anything at given pathfile");
    }
}//--------------------------------------------------------------------------------------------------------------------

void playMusic(MusicManager *currMusicMgr){
// Play the current music if it exists
    if(Mix_PlayMusic(currMusicMgr->currMusic, -1) == -1)
		reportErreur("playMusic(): cannot play it");
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
}//--------------------------------------------------------------------------------------------------------------------

void pauseMusic(MusicManager *currMusicMgr){
// Pause while playing the current music
    Mix_PauseMusic();
    //Mix_ResumeMusic();
}//--------------------------------------------------------------------------------------------------------------------

void destroyMusicManager(MusicManager *currMusicMgr){
// Free MusicManager memory
    int i;

    if (currMusicMgr->currMusic != NULL){
        Mix_FreeMusic(currMusicMgr->currMusic);
    }

    for(i = 0; i < currMusicMgr->allSoundSize; i++){
        Mix_FreeChunk(currMusicMgr->allSound[i]);
    }
    free(currMusicMgr->allSound);

    free(currMusicMgr);
}//--------------------------------------------------------------------------------------------------------------------

