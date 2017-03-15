#include "help_page.h"

//NB: Sub pages are not managed by gui_manager and can be called by another page
int displayHelpSubPage(SDL_Window *p_window, GameManager *currGameMgr){
    SDL_Surface *background1;
    Button *goBackArrow;

    int clicX, clicY;
    int jumpLineGapSize = 25;
    int pageTitleId = 27;
    int contentId[] ={20, 21, 22, 23, 24, 25, 26};
    int contentIdSize = sizeof(contentId) / sizeof(int);
    int i;
    int exitStatut = 1;

    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 15);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 20);
    TTF_Font *font3 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);
    goBackArrow = initButton("data/img/gui/buttons/arrow-back.txt");

    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos;
    SDL_Rect textPosDefault = { 100, 100, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect pageTitlePos = {300, 50, 0, 0};
    SDL_Event event;

    goBackArrow->posX = 5;
    goBackArrow->posY = 5;

    background1 = loadImage("data/img/background/sky2.jpg");

    while ( SDL_WaitEvent(&event) ){
        if(event.type == SDL_QUIT){ // Leave the game
            exitStatut = 0;
            break;
        }
        // Go back last page
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            break;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&clicX, &clicY);
            // Leaving by pressing button
            if( isClicked(goBackArrow, clicX, clicY)){
                break;
            }
        }

        textPos = textPosDefault;
//--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);

        // Printing the page title
        setTextLayout(p_window,
                        currGameMgr->translaManager->allTranslations[pageTitleId]->sentence[currGameMgr->translaManager->currLangageId],
                        currGameMgr->translaManager->allTranslations[pageTitleId]->sentenceSize, font3, textColor, pageTitlePos);
        // Printing page content
        for( i = 0; i < contentIdSize; i++){
            setTextLayout(p_window,
                        currGameMgr->translaManager->allTranslations[contentId[i]]->sentence[currGameMgr->translaManager->currLangageId],
                        currGameMgr->translaManager->allTranslations[contentId[i]]->sentenceSize, font2, textColor, textPos);
            textPos.y += jumpLineGapSize;
        }
        // Displaying back button
        displayButton(p_window, goBackArrow);
        // Indication to go back
        textPos.y = SDL_GetWindowSurface(p_window)->h - 120;
        setTextLayout(p_window,
                        currGameMgr->translaManager->allTranslations[12]->sentence[currGameMgr->translaManager->currLangageId],
                        currGameMgr->translaManager->allTranslations[12]->sentenceSize, font1, textColor, textPos);

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }

//-------------------------------- Free memory--------------------------------------------
    destroyButton(goBackArrow);
    SDL_FreeSurface(background1);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);

    return exitStatut;
}//--------------------------------------------------------------------------------------------------------------------
