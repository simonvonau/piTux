#ifndef __MUSICMANAGER_H__
#define __MUSICMANAGER_H__
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL_mixer.h>

#include "../utils/reporting.h"

typedef struct{
    int bufferLength;
    Mix_Music *currMusic;
    Mix_Chunk **allSound;
    int allSoundSize;
}MusicManager;


MusicManager *initMusicManager(int p_bufferLen);
void playSoundByID(MusicManager *p_musicMgr, int p_idSound);
void loadMusic(MusicManager *p_musicMgr, char* p_path);
void playMusic(MusicManager *p_musicMgr);
void pauseMusic(MusicManager *p_musicMgr);
void destroyMusicManager(MusicManager *p_musicMgr);
#endif
