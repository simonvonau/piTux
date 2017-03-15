#include "gamePage.h"

int displayGamePage(SDL_Window *p_window, char *levelPath, int levelPathSize, GameManager *currGameMgr){
    SDL_Surface *background1, *lifesLeft, *coinLeft, *timeLeft, *redBackground;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);
    SDL_Event event, lastEvent;

    SDL_Color textColor = {0, 0, 0};
    SDL_Color timeLeftColor = {0, 0, 0};
    SDL_Rect textPos = { 300, 100, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect coinPos = { SDL_GetWindowSurface(p_window)->w-80, 30, 0, 0};
    SDL_Rect coinPosText = { SDL_GetWindowSurface(p_window)->w-40, 35, 0, 0};
    SDL_Rect lifeLeftPos = { SDL_GetWindowSurface(p_window)->w-80, 0, 0, 0};
    SDL_Rect lifeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40, 5, 0, 0};
    SDL_Rect timeLeftPos = { SDL_GetWindowSurface(p_window)->w-80, 60, 0, 0};
    SDL_Rect timeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40, 65, 0, 0};

    int exitStatut = 0;
    int i;
    int deplaX = 0;
    int deplaY = 0;
    char temp_str[10];

    // To display a red screen when time is running out
    int displayTimeWarning = 0;
    int warningTimeLaps = 1000;
    int warningLastTime = 0;
    int warningDuration = 200;

    // Fps management
    int lastTime = SDL_GetTicks();
    int currTime = lastTime;
    int loopTime;
    int currFPS = 0;
    int countTime = 2000; // To process average looptime
    int sumTime = 0; // Sum of loopTime
    int sumLoopDone = 0; // Sum of each loop

    // Init level
    loadLevelByGameMgr(currGameMgr, levelPath, levelPathSize);

    background1 = loadImage("data/img/background/arctis.jpg");
    lifesLeft = loadImage("data/img/icon/life_left.png");
    coinLeft = loadImage("data/img/icon/coin.png");
    timeLeft = loadImage("data/img/icon/time.png");
    redBackground = loadImage("data/img/background/red_background.png");

    SDL_FlushEvent(SDL_KEYDOWN);
    while ( 1 ){
//--------------------------Events management-----------------------------------------------------------------------
        SDL_PollEvent(&event);
        // Escape => break mode
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && event.key.keysym.sym != lastEvent.key.keysym.sym){
            if (!(exitStatut=displayBreakSubPage(p_window, currGameMgr))){ // Leave the game
                break;
            }
            if (exitStatut != 1000){// Go to another page
                break;
            }
            if (exitStatut == 1000){
                //Just keep playing
            }
            //lastTime = SDL_GetTicks() - 5;
        }
        if(event.type == SDL_QUIT){
            exitStatut = 0;// Leave the game
            break;
        }
//--------------------------Management-----------------------------------------------------------------------
        currTime = SDL_GetTicks();
        loopTime = currTime - lastTime;
        lastTime = currTime;
        currFPS = 1000 / loopTime;


        sumTime += loopTime;
        sumLoopDone +=1;
        if (countTime < sumTime){
            printf("average frame duration: %f, fps: %f \n", 1.0 * sumTime / sumLoopDone, 1000.0 / (1.0 * sumTime / sumLoopDone));
            sumLoopDone = 0;
            sumTime = 0;
        }

        // When game windows is not focus loopTime causes some collisions bugs
        if (loopTime < 80){
            refreshGameByGameManager(currGameMgr, loopTime, SDL_GetWindowSurface(p_window)->w,SDL_GetWindowSurface(p_window)->h, deplaX, deplaY);


            // Level time management (fail when time <= 0)
            /*currLevelManager->currLevel->currTime -= loopTime;
            if(currLevelManager->currLevel->currTime <= 40000){timeLeftColor.r = 128;}
            if(currLevelManager->currLevel->currTime <= 20000){
                timeLeftColor.r = 255;
                warningLastTime += loopTime;
                if( warningLastTime >= warningTimeLaps){
                    warningLastTime = 0;
                    displayTimeWarning = 1;
                }
                if(warningDuration < warningLastTime){
                    displayTimeWarning = 0;
                }
            }
            if( currLevelManager->currLevel->currTime <= 0){
                // Fail
                currLevelManager->currLevel->currTime = 0;
            }*/
        }else{
            // Pause mode if game windows is not focus
            // Conflict problem after break mode
            //if (!(exitStatut=displayBreakSubPage(p_window, currGameMgr))){
              //  break;
            //}
        }




//--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        // Displaying Level
        displayLevelByLevelManager(currGameMgr->levelManager->currLevel, p_window, 1, deplaX, deplaY
                     ,currGameMgr->allBlocks, currGameMgr->allBonus, currGameMgr->allEnemies);

        displayHeros(currGameMgr->herosMgr->heros, p_window);

        // Lifes, coins and timeleft
        SDL_BlitSurface(lifesLeft, NULL, SDL_GetWindowSurface(p_window), &lifeLeftPos);
        SDL_BlitSurface(coinLeft, NULL, SDL_GetWindowSurface(p_window), &coinPos);
        SDL_BlitSurface(timeLeft, NULL, SDL_GetWindowSurface(p_window), &timeLeftPos);
        sprintf(temp_str, "%d", currGameMgr->herosMgr->heros->lifesLeft);
        setTextLayout(p_window, temp_str, 5, font1, textColor, lifeLeftPosText);
        sprintf(temp_str, "%d", currGameMgr->herosMgr->heros->nbCoins);
        setTextLayout(p_window, temp_str, 5, font1, textColor, coinPosText);
        //setTextLayout(p_window, setTimeLayout(currLevelManager->currLevel->currTime,4)
                      //, 4, font1, timeLeftColor, timeLeftPosText);

        // Warning when time is running out
        if (displayTimeWarning){
            SDL_BlitSurface(redBackground, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        }

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
        lastEvent = event;
    }
//-------------------------------- Free memory--------------------------------------------
    destroyLevel(currGameMgr->levelManager->currLevel);
    SDL_FreeSurface(background1);
    SDL_FreeSurface(redBackground);
    SDL_FreeSurface(lifesLeft);
    SDL_FreeSurface(coinLeft);
    SDL_FreeSurface(timeLeft);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return exitStatut;
}//--------------------------------------------------------------------------------------------------------------------
