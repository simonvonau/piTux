#include "mainPage.h"

int displaySettingsPage(SDL_Window *p_window, GameManager *p_gameMgr){
    SDL_Surface *cursor, *background1, *background2;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 25);
    TTF_Font *font3 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 15);
    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { 100,150, 0, 0};
    SDL_Rect nullPos = { 0,0, 0, 0};
    SDL_Rect pageTitlePos = {200, 50, 0, 0};
    SDL_Event event;

    int nextPage = 1;
    int userChoice = 0;
    int nbMaxUserChoice = p_gameMgr->translaManager->nbLanguages;
    int pageTitleId = 13;
    int i;

    background1 = loadImage("data/img/background/arctis.jpg");
    background2 = loadImage("data/img/background/frame.png");
    cursor = loadImage("data/img/cursor/cursor.png");

    // Loading ambiance music
    loadMusic(currMusicManager, "data/music/forest2.ogg");
    playMusic(currMusicManager);

    while ( SDL_WaitEvent(&event) ){
        // Events management
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){// Change userChoice
            userChoice -= 1;
            playSoundByID(currMusicManager, 13);
        }else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN){// Change userChoice
            userChoice += 1;
            playSoundByID(currMusicManager, 13);
        }
        if(event.type == SDL_QUIT){
            nextPage = 0;// Leave the game
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            nextPage = 1; // Go back to main menu
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){// Validate userChoice
            playSoundByID(currMusicManager, 14);
            p_gameMgr->translaManager->currLanguageId = userChoice;
            nextPage = 1;
            break;
        }

        // Set a limit to userChoice
        userChoice = userChoice % nbMaxUserChoice;
        if(userChoice < 0){
            userChoice = nbMaxUserChoice - 1;
        }

        // Laying out
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        SDL_BlitSurface(background2, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        // Printing the page title
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentenceSize, font2, textColor, pageTitlePos);

        // Print available languages
        for(i=0; i < nbMaxUserChoice; i++){
            if(userChoice == i){
                // Print the current selected choice
                setTextLayout(p_window,
                        p_gameMgr->translaManager->languagesList->sentence[i],
                        p_gameMgr->translaManager->languagesList->sentenceSize, font2, textColor, textPos);
                textPos.x -= 35;
                // Print the cursor
                SDL_BlitSurface(cursor,NULL,SDL_GetWindowSurface(p_window),&textPos);
                textPos.x += 35;
            }else{
                setTextLayout(p_window,
                        p_gameMgr->translaManager->languagesList->sentence[i],
                        p_gameMgr->translaManager->languagesList->sentenceSize, font1, textColor, textPos);
            }
            textPos.y += 45;
        }

        // Indication to go back
        textPos.y = SDL_GetWindowSurface(p_window)->h - 120;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[12]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[12]->sentenceSize, font3, textColor, textPos);
        textPos.y = 150;

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }

    // Free memory
    SDL_FreeSurface(background1);
    SDL_FreeSurface(background2);
    SDL_FreeSurface(cursor);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    return nextPage;
}//--------------------------------------------------------------------------------------------------------------------
