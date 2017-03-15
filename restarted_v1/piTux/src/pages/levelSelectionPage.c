#include "levelSelectionPage.h"

int chooseLevel(LevelIcon **allLevels, int nbLevels, int userChoice, char *keyPressed){
// Select the next level to play
    // Global tests
    if(allLevels[userChoice]->linkedLevel1 <= -1
        && allLevels[userChoice]->linkedLevel2 <= -1){
        return userChoice;
    }
    if(allLevels[userChoice]->linkedLevel1 <= -1
        && allLevels[userChoice]->linkedLevel2 >= 0){
        return allLevels[userChoice]->linkedLevel2;
    }
    if(allLevels[userChoice]->linkedLevel1 >= 0
        && allLevels[userChoice]->linkedLevel2 <= -1){
        return allLevels[userChoice]->linkedLevel1;
    }

    // Tests for each key pressed:
    if( strcmp(keyPressed,"LEFT") == 0){
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX < allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX > allLevels[userChoice]->posX){
            return allLevels[userChoice]->linkedLevel1;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX > allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX < allLevels[userChoice]->posX){
            return allLevels[userChoice]->linkedLevel2;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX >= allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX >= allLevels[userChoice]->posX){
            return userChoice;
        }
        if(fabs(allLevels[allLevels[userChoice]->linkedLevel1]->posY - allLevels[userChoice]->posY)
           < fabs(allLevels[allLevels[userChoice]->linkedLevel2]->posY > allLevels[userChoice]->posY)){
            return allLevels[userChoice]->linkedLevel1;
        }else{
            return allLevels[userChoice]->linkedLevel2;
        }
    }
    if( strcmp(keyPressed,"RIGHT") == 0){
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX < allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX > allLevels[userChoice]->posX){
            return allLevels[userChoice]->linkedLevel2;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX > allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX < allLevels[userChoice]->posX){
            return allLevels[userChoice]->linkedLevel1;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posX <= allLevels[userChoice]->posX
           && allLevels[allLevels[userChoice]->linkedLevel2]->posX <= allLevels[userChoice]->posX){
            return userChoice;
        }
        if(fabs(allLevels[allLevels[userChoice]->linkedLevel1]->posY - allLevels[userChoice]->posY)
           < fabs(allLevels[allLevels[userChoice]->linkedLevel2]->posY > allLevels[userChoice]->posY)){
            return allLevels[userChoice]->linkedLevel1;
        }else{
            return allLevels[userChoice]->linkedLevel2;
        }
    }
    if( strcmp(keyPressed,"UP") == 0){
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY < allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY > allLevels[userChoice]->posY){
            return allLevels[userChoice]->linkedLevel1;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY > allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY < allLevels[userChoice]->posY){
            return allLevels[userChoice]->linkedLevel2;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY >= allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY >= allLevels[userChoice]->posY){
            return userChoice;
        }
        if(fabs(allLevels[allLevels[userChoice]->linkedLevel1]->posX - allLevels[userChoice]->posX)
           < fabs(allLevels[allLevels[userChoice]->linkedLevel2]->posX > allLevels[userChoice]->posX)){
            return allLevels[userChoice]->linkedLevel2;
        }else{
            return allLevels[userChoice]->linkedLevel1;
        }
    }
    if( strcmp(keyPressed,"DOWN") == 0){
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY < allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY > allLevels[userChoice]->posY){
            return allLevels[userChoice]->linkedLevel2;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY > allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY < allLevels[userChoice]->posY){
            return allLevels[userChoice]->linkedLevel1;
        }
        if( allLevels[allLevels[userChoice]->linkedLevel1]->posY <= allLevels[userChoice]->posY
           && allLevels[allLevels[userChoice]->linkedLevel2]->posY <= allLevels[userChoice]->posY){
            return userChoice;
        }
        if(fabs(allLevels[allLevels[userChoice]->linkedLevel1]->posX - allLevels[userChoice]->posX)
           < fabs(allLevels[allLevels[userChoice]->linkedLevel2]->posX > allLevels[userChoice]->posX)){
            return allLevels[userChoice]->linkedLevel2;
        }else{
            return allLevels[userChoice]->linkedLevel1;
        }
    }

    return userChoice;
}//--------------------------------------------------------------------------------------------------------------------

int displayLevelSelectionPage(SDL_Window *p_window, char *nextLevelPath, GameManager *currGameMgr){
    SDL_Surface *cursor, *background1, *greenDot, *lifesLeft, *coinLeft;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    SDL_Event event;

    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { 300,100, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect levelDotPos = { 0,0, 100, 100};
    SDL_Rect coinPos = { SDL_GetWindowSurface(p_window)->w-80,30, 0, 0};
    SDL_Rect coinPosText = { SDL_GetWindowSurface(p_window)->w-40,35, 0, 0};
    SDL_Rect lifeLeftPos = { SDL_GetWindowSurface(p_window)->w-80,0, 0, 0};
    SDL_Rect lifeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40,5, 0, 0};

    int nbLevels = 5;
    LevelIcon **allLevels = initLevelIcons("data/world1/world1.txt", &nbLevels);
    int nextPage = 0;
    int userChoice = 0;
    int newChoice = 0;
    int pathSize = 512;

    int i;
    char temp_str[10];

    background1 = loadImage("data/img/background/world1.jpg");
    greenDot = loadImage("data/img/icon/green_dot.png");
    lifesLeft = loadImage("data/img/icon/life_left.png");
    coinLeft = loadImage("data/img/icon/coin.png");
    cursor= loadImage("data/img/cursor/tux_cursor.png");

    // Loading ambiance music
    //loadMusic(CurrMusicManager, "data/music/salcon.ogg");
    //playMusic(CurrMusicManager);

    while ( SDL_WaitEvent(&event) ){
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
            strncpy(nextLevelPath, allLevels[userChoice]->path, pathSize );
            break;
        }

        // Set a limit to userChoice
        newChoice= newChoice%nbLevels;
        if(newChoice<0){
            newChoice=nbLevels-1;
        }
        if(allLevels[newChoice]->isAvailable){
            userChoice = newChoice;
        }


//--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        SDL_BlitSurface(lifesLeft, NULL, SDL_GetWindowSurface(p_window), &lifeLeftPos);
        SDL_BlitSurface(coinLeft, NULL, SDL_GetWindowSurface(p_window), &coinPos);

        // Lifes and coins
        /*sprintf(temp_str, "%d", currGameMgr->herosMgr->heros->lifesLeft);
        setTextLayout(p_window, temp_str, 5, font1, textColor, lifeLeftPosText);
        sprintf(temp_str, "%d", currGameMgr->herosMgr->heros->nbCoins);
        setTextLayout(p_window, temp_str, 5, font1, textColor, coinPosText);*/

        for(i=0; i < nbLevels; i++){
            levelDotPos.x = allLevels[i]->posX;
            levelDotPos.y = allLevels[i]->posY;
            if(allLevels[i]->isAvailable){
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
    for(i=0; i < nbLevels; i++){
        destroyLevelIcon(allLevels[i]);
    }
    destroyLevelIcon(allLevels);

    SDL_FreeSurface(background1);
    SDL_FreeSurface(greenDot);
    SDL_FreeSurface(lifesLeft);
    SDL_FreeSurface(coinLeft);
    SDL_FreeSurface(cursor);
    TTF_CloseFont(font1);
    return nextPage;
}//--------------------------------------------------------------------------------------------------------------------
