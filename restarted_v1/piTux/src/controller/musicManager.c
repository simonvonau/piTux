#include "musicManager.h"



MusicManager *initMusicManager(int p_bufferLen, int p_channelsQty){
// Create a new MusicManager
    MusicManager *res = malloc(sizeof(MusicManager));
    if (res == NULL){
        reportErreur("initMusicManager(): malloc() error");
    }
    Mix_AllocateChannels(p_channelsQty);
    Mix_Volume(0, MIX_MAX_VOLUME/2);
    res->bufferLength = p_bufferLen;
    res->currMusic = NULL;

    // Loading all sounds
    /*res->allSoundSize = 1;
    res->allSound = malloc( res->allSoundSize * sizeof(Mix_Chunk *));
    if( res->allSound == NULL ){
        reportErreur("initMusicManager(): malloc() error2");
    }*/

    return res;
}//--------------------------------------------------------------------------------------------------------------------

void loadMusic(MusicManager *p_musicMgr, char* p_path){
// Load an other music from pathfile
    if (p_musicMgr->currMusic != NULL){
        Mix_FreeMusic(p_musicMgr->currMusic);
    }
    if(!(p_musicMgr->currMusic = Mix_LoadMUS(p_path))){
        reportErreur("loadMusic(): cannot find anything at given pathfile");
    }
}//--------------------------------------------------------------------------------------------------------------------

void playMusic(MusicManager *p_musicMgr){
// Play the current music if it exists
    if(Mix_PlayMusic(p_musicMgr->currMusic, 1) == -1)
		reportErreur("playMusic(): cannot play it");
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
}//--------------------------------------------------------------------------------------------------------------------

void pauseMusic(MusicManager *p_musicMgr){
// Pause while playing the current music
    Mix_PauseMusic();
    //Mix_ResumeMusic();
}//--------------------------------------------------------------------------------------------------------------------

void destroyMusicManager(MusicManager *p_musicMgr){
// Free MusicManager memory
    int i;

    if (p_musicMgr->currMusic != NULL){
        Mix_FreeMusic(p_musicMgr->currMusic);
    }

    /*for(i = 0; i < p_musicMgr->allSoundSize; i++){
        Mix_FreeChunk(p_musicMgr->allSound[i]);
    }
    free(p_musicMgr->allSound);*/

    free(p_musicMgr);
}//--------------------------------------------------------------------------------------------------------------------

