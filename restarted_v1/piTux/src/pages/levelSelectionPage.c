#include "levelSelectionPage.h"

int chooseLevel(LevelIcon **p_allLevels, int p_nbLevels, int p_userChoice, char *p_keyPressed){
// Select the next level to play
    // Global tests
    if(p_allLevels[p_userChoice]->linkedLevel1 <= -1
        && p_allLevels[p_userChoice]->linkedLevel2 <= -1){
        return p_userChoice;
    }
    if(p_allLevels[p_userChoice]->linkedLevel1 <= -1
        && p_allLevels[p_userChoice]->linkedLevel2 >= 0){
        return p_allLevels[p_userChoice]->linkedLevel2;
    }
    if(p_allLevels[p_userChoice]->linkedLevel1 >= 0
        && p_allLevels[p_userChoice]->linkedLevel2 <= -1){
        return p_allLevels[p_userChoice]->linkedLevel1;
    }

    // Tests for each key pressed:
    if( strcmp(p_keyPressed,"LEFT") == 0){
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX < p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX > p_allLevels[p_userChoice]->posX){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX > p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX < p_allLevels[p_userChoice]->posX){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX >= p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX >= p_allLevels[p_userChoice]->posX){
            return p_userChoice;
        }
        if(fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY - p_allLevels[p_userChoice]->posY)
           < fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY > p_allLevels[p_userChoice]->posY)){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }else{
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
    }
    if( strcmp(p_keyPressed,"RIGHT") == 0){
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX < p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX > p_allLevels[p_userChoice]->posX){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX > p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX < p_allLevels[p_userChoice]->posX){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX <= p_allLevels[p_userChoice]->posX
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX <= p_allLevels[p_userChoice]->posX){
            return p_userChoice;
        }
        if(fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY - p_allLevels[p_userChoice]->posY)
           < fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY > p_allLevels[p_userChoice]->posY)){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }else{
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
    }
    if( strcmp(p_keyPressed,"UP") == 0){
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY < p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY > p_allLevels[p_userChoice]->posY){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY > p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY < p_allLevels[p_userChoice]->posY){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY >= p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY >= p_allLevels[p_userChoice]->posY){
            return p_userChoice;
        }
        if(fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX - p_allLevels[p_userChoice]->posX)
           < fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX > p_allLevels[p_userChoice]->posX)){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }else{
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
    }
    if( strcmp(p_keyPressed,"DOWN") == 0){
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY < p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY > p_allLevels[p_userChoice]->posY){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY > p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY < p_allLevels[p_userChoice]->posY){
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
        if( p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posY <= p_allLevels[p_userChoice]->posY
           && p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posY <= p_allLevels[p_userChoice]->posY){
            return p_userChoice;
        }
        if(fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel1]->posX - p_allLevels[p_userChoice]->posX)
           < fabs(p_allLevels[p_allLevels[p_userChoice]->linkedLevel2]->posX > p_allLevels[p_userChoice]->posX)){
            return p_allLevels[p_userChoice]->linkedLevel2;
        }else{
            return p_allLevels[p_userChoice]->linkedLevel1;
        }
    }

    return p_userChoice;
}//--------------------------------------------------------------------------------------------------------------------

int displayLevelSelectionPage(SDL_Window *p_window, char *p_nextLevelPath, GameManager *p_gameMgr){
    SDL_Surface *cursor, *background1, *greenDot, *lifesLeft, *coinLeft;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 18);
    SDL_Event event;

    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { 445,430, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect levelDotPos = { 0,0, 100, 100};
    SDL_Rect coinPos = { SDL_GetWindowSurface(p_window)->w-80,30, 0, 0};
    SDL_Rect coinPosText = { SDL_GetWindowSurface(p_window)->w-40,35, 0, 0};
    SDL_Rect lifeLeftPos = { SDL_GetWindowSurface(p_window)->w-80,0, 0, 0};
    SDL_Rect lifeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40,5, 0, 0};

    int nbLevels = p_gameMgr->levIconMgr->allLevIconsSize;
    LevelIcon **allLevels = p_gameMgr->levIconMgr->allLevIcons;
    int nextPage = 0;
    int userChoice = 0;
    int newChoice = p_gameMgr->herosMgr->heroInstance->currentLevelNo;
    int pathSize = 512;
    int isFirstLoop = 1; // To force the first loop even if there are no detected events

    int i;
    char temp_str[64];

    background1 = loadImage("data/img/background/world1.jpg");
    greenDot = loadImage("data/img/icon/green_dot.png");
    lifesLeft = loadImage("data/img/icon/life_left.png");
    coinLeft = loadImage("data/img/icon/coin.png");
    cursor= loadImage("data/img/cursor/tux_cursor.png");

    // Loading ambiance music
    //loadMusic(CurrMusicManager, "data/music/salcon.ogg");
    //playMusic(CurrMusicManager);

    while (isFirstLoop || SDL_WaitEvent(&event)){
        isFirstLoop = 0;
//--------------------------Events management-----------------------------------------------------------------------
        if( event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_UP){// Change userChoice
            newChoice = chooseLevel(allLevels, nbLevels, userChoice, "UP");
        }
        if (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_DOWN){// Change userChoice
            newChoice = chooseLevel(allLevels, nbLevels, userChoice, "DOWN");
        }
        if( event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_RIGHT){// Change userChoice
            newChoice = chooseLevel(allLevels, nbLevels, userChoice, "RIGHT");
        }
        if (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_LEFT){// Change userChoice
            newChoice = chooseLevel(allLevels, nbLevels, userChoice, "LEFT");
        }
        if(event.type== SDL_QUIT){
            nextPage = 0;// Leave the game
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            nextPage = 1; // Go back to main menu
            break;
        }
        if( event.type==SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){// Validate userChoice
            nextPage = 7;
            strncpy(p_nextLevelPath, allLevels[userChoice]->path, pathSize );
            break;
        }

        // Set a limit to userChoice
        newChoice = newChoice % nbLevels;
        if(newChoice < 0){
            newChoice = nbLevels - 1;
        }
        if(allLevels[newChoice]->isAvailable){
            userChoice = newChoice;
            p_gameMgr->herosMgr->heroInstance->currentLevelNo = newChoice;
        }

//--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        SDL_BlitSurface(lifesLeft, NULL, SDL_GetWindowSurface(p_window), &lifeLeftPos);
        SDL_BlitSurface(coinLeft, NULL, SDL_GetWindowSurface(p_window), &coinPos);

        // Lifes and coins
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->lifesLeft);
        setTextLayout(p_window, temp_str, 5, font1, textColor, lifeLeftPosText);
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->nbCoins);
        setTextLayout(p_window, temp_str, 5, font1, textColor, coinPosText);

        // Notification about the custom level
        setTextLayout(p_window, p_gameMgr->translaManager->allTranslations[7]->sentence[p_gameMgr->translaManager->currLanguageId]
                      , p_gameMgr->translaManager->allTranslations[7]->sentenceSize, font2, textColor, textPos);

        for(i=0; i < nbLevels; i++){
            levelDotPos.x = allLevels[i]->posX;
            levelDotPos.y = allLevels[i]->posY;
            if(allLevels[i]->isCleared){
                SDL_BlitSurface(greenDot, NULL, SDL_GetWindowSurface(p_window), &levelDotPos);
            }
            if( i == userChoice){
                levelDotPos.y -= 10;
                SDL_BlitSurface(cursor, NULL, SDL_GetWindowSurface(p_window), &levelDotPos);
            }
        }

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }

//-------------------------------- Free memory--------------------------------------------
    SDL_FreeSurface(background1);
    SDL_FreeSurface(greenDot);
    SDL_FreeSurface(lifesLeft);
    SDL_FreeSurface(coinLeft);
    SDL_FreeSurface(cursor);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return nextPage;
}//--------------------------------------------------------------------------------------------------------------------
