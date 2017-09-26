#include "creditsPage.h"

int displayCreditsPage(SDL_Window *p_window, GameManager *p_gameMgr){
    SDL_Surface *cursor, *background1, *background2;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);
    TTF_Font *font3 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 15);
    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { 100, 100, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect pageTitlePos = {300, 50, 0, 0};
    SDL_Event event;

    int nextPage = 1;
    int pageTitleId = 5;

    background1 = loadImage("data/img/background/oiltux.jpg");
    background2 = loadImage("data/img/background/frame.png");
    cursor= loadImage("data/img/cursor/cursor.png");
    // Loading ambiance music
    //loadMusic(CurrMusicManager, "data/music/credits.ogg");
    //playMusic(CurrMusicManager);

    while ( SDL_WaitEvent(&event) ){
        if(event.type == SDL_QUIT){
            nextPage = 0;// Leave the game
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            nextPage = 1; // Go back to main menu
            break;
        }

        // Laying out
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        SDL_BlitSurface(background2, NULL, SDL_GetWindowSurface(p_window), &nullPos);

        // Printing the page title
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentenceSize, font2, textColor, pageTitlePos);

        // Printing text content
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[29]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[29]->sentenceSize, font1, textColor, textPos);
        textPos.y += 80;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[9]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[9]->sentenceSize, font1, textColor, textPos);
        textPos.y += 80;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[10]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[10]->sentenceSize, font1, textColor, textPos);
        textPos.y += 80;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[11]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[11]->sentenceSize, font1, textColor, textPos);
        textPos.y += 80;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[33]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[33]->sentenceSize, font1, textColor, textPos);

        textPos.y = SDL_GetWindowSurface(p_window)->h - 120;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[12]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[12]->sentenceSize, font3, textColor, textPos);

        textPos.y = 100;

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
