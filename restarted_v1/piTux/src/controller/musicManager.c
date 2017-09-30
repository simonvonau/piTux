#include "musicManager.h"



MusicManager *initMusicManager(int p_bufferLen){
// Create a new MusicManager
    MusicManager *res = malloc(sizeof(MusicManager));
    int allSongPathSize = 15;
    char *allSongPath[] = {"./data/sounds/brick.wav", "./data/sounds/coin.wav", "./data/sounds/explosion.wav", "./data/sounds/fall.wav"
    , "./data/sounds/grow.ogg", "./data/sounds/fire-flower.wav", "./data/sounds/hurt.wav", "./data/sounds/iceblock_bump.wav"
    , "./data/sounds/jump.wav", "./data/sounds/kill.wav", "./data/sounds/lifeup.wav", "./data/sounds/shoot.wav", "./data/sounds/squish.wav"
    , "./data/sounds/ticking2.wav", "./data/sounds/upgrade.wav"};
    int i;
    char errorMsg[1024] = "error loading song : ";
    char strBuff[128];

    if (res == NULL){
        reportErreur("initMusicManager(): malloc() error");
    }
    Mix_AllocateChannels(allSongPathSize);
    res->bufferLength = p_bufferLen;
    res->currMusic = NULL;

    // Loading all sounds
    res->allSoundSize = allSongPathSize;
    res->allSound = malloc( res->allSoundSize * sizeof(Mix_Chunk *));
    if( res->allSound == NULL ){
        reportErreur("initMusicManager(): malloc() error2");
    }
    for(i = 0; i < res->allSoundSize; i++){
        res->allSound[i] = Mix_LoadWAV(allSongPath[i]);
        if(res->allSound[i] == NULL){
            sprintf(strBuff, "%d", i);
            strcat(errorMsg, strBuff);
            reportErreur(errorMsg);
        }
        Mix_Volume(i, MIX_MAX_VOLUME);
        Mix_VolumeChunk(res->allSound[i], MIX_MAX_VOLUME);
    }



    return res;
}//--------------------------------------------------------------------------------------------------------------------

void playSoundByID(MusicManager *p_musicMgr, int p_idSound){
// Play the short song : jump.wav
    if(Mix_PlayChannel( p_idSound, p_musicMgr->allSound[p_idSound], 0 ) == -1){
        printf("errors playSoundByID(...) : %s", Mix_GetError());
    }
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
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME / 2);
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

    for(i = 0; i < p_musicMgr->allSoundSize; i++){
        Mix_FreeChunk(p_musicMgr->allSound[i]);
    }
    free(p_musicMgr->allSound);

    free(p_musicMgr);
}//--------------------------------------------------------------------------------------------------------------------

