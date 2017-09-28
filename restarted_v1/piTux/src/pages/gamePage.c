#include "gamePage.h"

int displayGamePage(SDL_Window *p_window, char *p_levelPath, int p_levelPathSize, GameManager *p_gameMgr){
    SDL_Surface *background1, *lifesLeft, *coinLeft, *timeLeft, *redBackground, *endLevelCursor, *endLevelFrame;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);
    SDL_Event event, lastEvent;

    SDL_Color textColor = {0, 0, 0};
    SDL_Color textColor2 = {255, 255, 255};
    SDL_Color timeLeftColor = {0, 0, 0};
    SDL_Rect textPosCongrat = { 300, 170, 0, 0};
    SDL_Rect textPosTime = { 200, 220, 0, 0};
    SDL_Rect textPosEne = { 200, 250, 0, 0};
    SDL_Rect textPosFPS = { 200, 280, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect coinPos = { SDL_GetWindowSurface(p_window)->w-80, 30, 0, 0};
    SDL_Rect coinPosText = { SDL_GetWindowSurface(p_window)->w-40, 35, 0, 0};
    SDL_Rect lifeLeftPos = { SDL_GetWindowSurface(p_window)->w-80, 0, 0, 0};
    SDL_Rect lifeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40, 5, 0, 0};
    SDL_Rect timeLeftPos = { SDL_GetWindowSurface(p_window)->w-80, 60, 0, 0};
    SDL_Rect timeLeftPosText = { SDL_GetWindowSurface(p_window)->w-40, 65, 0, 0};
    SDL_Rect endLevelPosCurr = { 0, 0, 0, 0};
    SDL_Rect endLevelPosDefault = { 0, 0, 0, 0};
    SDL_Rect endLevelFramePos = {150, 150, 300, 100};

    int herosToLeftScreenBorder = 200;
    int herosToBottomScreenBorder = 200;
    int exitStatut = 0;
    int i;
    int cameraX = 0;
    int cameraY = 0;
    char temp_str[128];
    char temp_str2[128];
    int isTuxInsideIgloo = 0;
    int iglooEntranceDecal = 180; // The space between the igloo sprite positionX and its entrance
    int isLevelCleared = 0;
    int timeBeforeExiting = 5000;
    int nbEnemiesAtStart = 0;
    int timeAtEnd = 0;



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
    long sumTime = 0; // Sum of loopTime
    long sumLoopDone = 0; // Sum of each loop
    int averageFPSAtLevelEnd = -1;

    // Init level
    loadLevelByGameMgr(p_gameMgr, p_levelPath, p_levelPathSize);

    nbEnemiesAtStart = p_gameMgr->levelManager->currLevel->enemyInstancesSize;

    background1 = loadImage("data/img/background/arctis.jpg");
    lifesLeft = loadImage("data/img/icon/life_left.png");
    coinLeft = loadImage("data/img/icon/coin.png");
    timeLeft = loadImage("data/img/icon/time.png");
    redBackground = loadImage("data/img/background/red_background.png");
    endLevelCursor = loadImage("data/img/cursor/level_end.png");
    endLevelFrame = loadImage("data/img/background/score-backdrop.png");

    endLevelPosDefault.x = p_gameMgr->levelManager->currLevel->finishPosX;
    endLevelPosDefault.y = SDL_GetWindowSurface(p_window)->h - endLevelCursor->h - 96;

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
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f){
            p_gameMgr->herosMgr->heroInstance->fireKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_f){
            p_gameMgr->herosMgr->heroInstance->fireKeyPressed = 0;
            p_gameMgr->herosMgr->heroInstance->hasReleaseFireKey = 1;
        }

        // Jump
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){
            p_gameMgr->herosMgr->heroInstance->jumpKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP){
            p_gameMgr->herosMgr->heroInstance->jumpKeyPressed = 0;
            p_gameMgr->herosMgr->heroInstance->hasReleaseJumpKey = 1;
        }

        // Move to left
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT){
            p_gameMgr->herosMgr->heroInstance->leftKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT){
            p_gameMgr->herosMgr->heroInstance->leftKeyPressed = 0;
        }
        // Move to right
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT){
            p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 1;
        }else if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT){
            p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 0;
        }
    //--------------------------Management-----------------------------------------------------------------------
        // Time management
        currTime = SDL_GetTicks();
        loopTime = currTime - lastTime;
        lastTime = currTime;
        currFPS = 1000 / loopTime;
        // Average fps process
        sumTime += loopTime;
        sumLoopDone +=1;
        if(isLevelCleared && averageFPSAtLevelEnd < 0){
            averageFPSAtLevelEnd = (int)(1000.0 / (1.0 * sumTime / sumLoopDone));
            timeAtEnd = p_gameMgr->levelManager->currLevel->timeLeft;
        }

        // Tux dies if timeleft <= 0
        if(!p_gameMgr->herosMgr->heroInstance->isDead && !isLevelCleared && p_gameMgr->levelManager->currLevel->timeLeft <= 0){
            heroInstanceDeath(p_gameMgr->herosMgr->heroInstance, p_gameMgr->herosMgr->heros, currTime);
        }

        // Updating the camera position
        if(!p_gameMgr->herosMgr->heroInstance->isDead){
            cameraX = p_gameMgr->herosMgr->heroInstance->posX - herosToLeftScreenBorder;
            cameraY = p_gameMgr->herosMgr->heroInstance->posY - herosToBottomScreenBorder;
        }

        // Updating the coord of the igloo at level end
        endLevelPosCurr.x = endLevelPosDefault.x - cameraX;
        endLevelPosCurr.y = endLevelPosDefault.y + cameraY;

        // When tux come at level end
        if(p_gameMgr->herosMgr->heroInstance->posX >= endLevelPosDefault.x){
            isLevelCleared = 1;
            p_gameMgr->herosMgr->heroInstance->leftKeyPressed = 0;
            p_gameMgr->herosMgr->heroInstance->jumpKeyPressed = 0;
            p_gameMgr->herosMgr->heroInstance->fireKeyPressed = 0;

            // When tux enters the igloo his sprite has to stop being displayed
            if(p_gameMgr->herosMgr->heroInstance->posX >= endLevelPosDefault.x + iglooEntranceDecal){
                isTuxInsideIgloo = 1;
            }

            // At level end tux has to go right until he enters his igloo
            if(isTuxInsideIgloo){
                p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 0;
            }else{
                p_gameMgr->herosMgr->heroInstance->rightKeyPressed = 1;
            }
        }

        // Back to level menu after a while
        if(isLevelCleared){
            timeBeforeExiting -= loopTime;
            if(timeBeforeExiting <= 0){
                exitStatut = 2;
                break;
            }
        }




        // When game windows is not focus the loopTime become too big and causes some collisions bugs
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
        // If tux goes out of the screen after he died => back to level menu
        if(p_gameMgr->herosMgr->heroInstance->isDead && - p_gameMgr->herosMgr->heroInstance->posY - herosToBottomScreenBorder > 0 ){
            if(p_gameMgr->herosMgr->heroInstance->lifesLeft >= 0){
                exitStatut = 2;
                break;
            }else{

            }

        }


    //--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);

        // Displaying Level
        displayLevelByLevelManager(p_gameMgr->levelManager->currLevel, p_window, 1, cameraX, cameraY
                     ,p_gameMgr->allBlocks, p_gameMgr->allBonus, p_gameMgr->allEnemies, p_gameMgr->fireBullet);

        // Displaying the end of level
        SDL_BlitSurface(endLevelCursor, NULL, SDL_GetWindowSurface(p_window), &endLevelPosCurr);

        if(!isTuxInsideIgloo){
            if(p_gameMgr->herosMgr->heroInstance->isDead){
                displayHeros(p_gameMgr->herosMgr, p_window, herosToLeftScreenBorder, p_gameMgr->herosMgr->heroInstance->posY + herosToBottomScreenBorder);
            }else{
                displayHeros(p_gameMgr->herosMgr, p_window, herosToLeftScreenBorder, herosToBottomScreenBorder);
            }
        }



        // Lifes, coins and timeleft
        SDL_BlitSurface(lifesLeft, NULL, SDL_GetWindowSurface(p_window), &lifeLeftPos);
        SDL_BlitSurface(coinLeft, NULL, SDL_GetWindowSurface(p_window), &coinPos);
        SDL_BlitSurface(timeLeft, NULL, SDL_GetWindowSurface(p_window), &timeLeftPos);
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->lifesLeft);
        setTextLayout(p_window, temp_str, 5, font1, textColor, lifeLeftPosText);
        sprintf(temp_str, "%d", p_gameMgr->herosMgr->heroInstance->nbCoins);
        setTextLayout(p_window, temp_str, 5, font1, textColor, coinPosText);
        if(isLevelCleared){
            setTextLayout(p_window, setTimeLayout(timeAtEnd, 4), 4, font1, timeLeftColor, timeLeftPosText);
        }else{
            setTextLayout(p_window, setTimeLayout(p_gameMgr->levelManager->currLevel->timeLeft, 4), 4, font1, timeLeftColor, timeLeftPosText);
        }

        // Warning when time is running out
        if (displayTimeWarning){
            SDL_BlitSurface(redBackground, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        }


        // The frame with the score at the end of the level
        if(isLevelCleared){
            SDL_BlitSurface(endLevelFrame, NULL, SDL_GetWindowSurface(p_window), &endLevelFramePos);

            // Congratulation message
            setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[36]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[36]->sentenceSize, font2, textColor2, textPosCongrat);
            // Time left
            setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[37]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[37]->sentenceSize, font1, textColor2, textPosTime);
            textPosTime.x += 180;
            setTextLayout(p_window, setTimeLayout(timeAtEnd, 5), 8, font1, textColor2, textPosTime);
            textPosTime.x -= 180;

            // Killed enemies
            setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[38]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[38]->sentenceSize, font1, textColor2, textPosEne);


            sprintf(temp_str, "%d", nbEnemiesAtStart - p_gameMgr->levelManager->currLevel->enemyInstancesSize);
            strcat(temp_str, "/");
            sprintf(temp_str2, "%d", nbEnemiesAtStart);
            strcat(temp_str, temp_str2);
            textPosEne.x += 180;
            setTextLayout(p_window, temp_str, 8, font1, textColor2, textPosEne);
            textPosEne.x -= 180;

            // Average FPS
            setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[39]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[39]->sentenceSize, font1, textColor2, textPosFPS);
            sprintf(temp_str, "%d", averageFPSAtLevelEnd);
            textPosFPS.x += 180;
            setTextLayout(p_window, temp_str, 8, font1, textColor2, textPosFPS);
            textPosFPS.x -= 180;
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
    SDL_FreeSurface(endLevelCursor);
    SDL_FreeSurface(endLevelFrame);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return exitStatut;
}//--------------------------------------------------------------------------------------------------------------------
