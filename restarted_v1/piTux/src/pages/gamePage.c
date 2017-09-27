#include "gamePage.h"

int displayGamePage(SDL_Window *p_window, char *p_levelPath, int p_levelPathSize, GameManager *p_gameMgr){
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

    int herosToLeftScreenBorder = 200;
    int herosToBottomScreenBorder = 200;
    int exitStatut = 0;
    int i;
    int cameraX = 0;
    int cameraY = 0;
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
    loadLevelByGameMgr(p_gameMgr, p_levelPath, p_levelPathSize);

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
            if (!(exitStatut=displayBreakSubPage(p_window, p_gameMgr))){ // Leave the game
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
        // Leave the game
        if(event.type == SDL_QUIT){
            exitStatut = 0;
            break;
        }

        // Shoot
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
            p_gameMgr->herosMgr->heroInstance->fireKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE){
            p_gameMgr->herosMgr->heroInstance->fireKeyPressed = 0;
        }

        // Jump
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){
            p_gameMgr->herosMgr->heroInstance->jumpKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP){
            p_gameMgr->herosMgr->heroInstance->jumpKeyPressed = 0;
        }
        // Move to right
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT){
            p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT){
            p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 0;
        }
        // Move to left
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT){
            p_gameMgr->herosMgr->heroInstance->leftKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT){
            p_gameMgr->herosMgr->heroInstance->leftKeyPressed = 0;
        }
    //--------------------------Management-----------------------------------------------------------------------
        currTime = SDL_GetTicks();
        loopTime = currTime - lastTime;
        lastTime = currTime;
        currFPS = 1000 / loopTime;

        // Time management
        sumTime += loopTime;
        sumLoopDone +=1;
        if (countTime < sumTime){
            printf("average frame duration: %f, fps: %f \n", 1.0 * sumTime / sumLoopDone, 1000.0 / (1.0 * sumTime / sumLoopDone));
            sumLoopDone = 0;
            sumTime = 0;
        }

        // Update the camera position
        cameraX = p_gameMgr->herosMgr->heroInstance->posX - herosToLeftScreenBorder;
        cameraY = p_gameMgr->herosMgr->heroInstance->posY - herosToBottomScreenBorder;



        // When game windows is not focus loopTime causes some collisions bugs
        if (loopTime < 80){
            refreshGameByGameManager(p_gameMgr, currTime, loopTime, SDL_GetWindowSurface(p_window)->w,SDL_GetWindowSurface(p_window)->h, cameraX, cameraY);


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
            //if (!(exitStatut=displayBreakSubPage(p_window, p_gameMgr))){
              //  break;
            //}
        }




    //--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);

        // Displaying Level
        displayLevelByLevelManager(p_gameMgr->levelManager->currLevel, p_window, 1, cameraX, cameraY
                     ,p_gameMgr->allBlocks, p_gameMgr->allBonus, p_gameMgr->allEnemies, p_gameMgr->fireBullet);

        displayHeros(p_gameMgr->herosMgr, p_window, herosToLeftScreenBorder, herosToBottomScreenBorder);

        // Lifes, coins and timeleft
        SDL_BlitSurface(lifesLeft, NULL, SDL_GetWindowSurface(p_window), &lifeLeftPos);
        SDL_BlitSurface(coinLeft, NULL, SDL_GetWindowSurface(p_window), &coinPos);
        SDL_BlitSurface(timeLeft, NULL, SDL_GetWindowSurface(p_window), &timeLeftPos);
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->lifesLeft);
        setTextLayout(p_window, temp_str, 5, font1, textColor, lifeLeftPosText);
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->nbCoins);
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
    destroyLevelByGameManager(p_gameMgr);
    SDL_FreeSurface(background1);
    SDL_FreeSurface(redBackground);
    SDL_FreeSurface(lifesLeft);
    SDL_FreeSurface(coinLeft);
    SDL_FreeSurface(timeLeft);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return exitStatut;
}//--------------------------------------------------------------------------------------------------------------------
