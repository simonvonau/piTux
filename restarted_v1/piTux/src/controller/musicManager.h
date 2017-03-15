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


MusicManager *initMusicManager(int bufferLength, int channelsQty);
void loadMusic(MusicManager *currMusicMgr, char* path);
void playMusic(MusicManager *currMusicMgr);
void pauseMusic(MusicManager *currMusicMgr);
void destroyMusicManager(MusicManager *currMusicMgr);
#endif
