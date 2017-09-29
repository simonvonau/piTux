#include "mainPage.h"

int displayMainPage(SDL_Window *p_window, GameManager *p_gameMgr){
    SDL_Surface *cursor, *background1, *background2;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 25);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);
    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { 300,100, 0, 0};
    SDL_Rect nullPos = { 0,0, 0, 0};
    SDL_Event event;

    int userChoice = 0;
    int nbMaxUserChoice = 5;
    int allUserChoicesId[]={2, 3, 4, 5, 6};
    int i;

    background1 = loadImage("data/img/background/arctis.jpg");
    background2 = loadImage("data/img/background/frame.png");
    cursor= loadImage("data/img/cursor/cursor.png");

    // Loading ambiance music
    loadMusic(currMusicManager, "data/music/intro.ogg");
    playMusic(currMusicManager);

    while ( SDL_WaitEvent(&event) ){
        // Events management
        if(event.type == SDL_QUIT){
            userChoice = 0;// Leave the game
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){// Change userChoice
            userChoice -= 1;
        }else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN){// Change userChoice
            userChoice += 1;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){// Validate userChoice
            // To match with gui_manager() switch's index
            userChoice += 2;
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

        for(i=0; i < nbMaxUserChoice; i++){
            if(userChoice == i){
                // Print the current selected choice
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[allUserChoicesId[i]]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[allUserChoicesId[i]]->sentenceSize, font2, textColor, textPos);
                textPos.x -= 35;
                // Print the cursor
                SDL_BlitSurface(cursor,NULL,SDL_GetWindowSurface(p_window),&textPos);
                textPos.x +=35;
            }else{
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[allUserChoicesId[i]]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[allUserChoicesId[i]]->sentenceSize, font1, textColor, textPos);
            }
            textPos.y += 45;
        }
        textPos.y -= 45 * nbMaxUserChoice;
        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }
    // Free memory
    SDL_FreeSurface(background1);
    SDL_FreeSurface(background2);
    SDL_FreeSurface(cursor);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return userChoice;
}//--------------------------------------------------------------------------------------------------------------------
