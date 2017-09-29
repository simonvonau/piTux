#include "break_page.h"

//NB: Sub pages are not managed by gui_manager and can be called by another page
int displayBreakSubPage(SDL_Window *p_window, GameManager *p_gameMgr){
    SDL_Surface *background1, *cursor;

    int jumpLineGapSize = 45;
    int pageTitleId = 17;
    // the Id of each displayed text on this page
    int contentId[] ={35, 18, 19, 34};
    int contentIdSize = sizeof(contentId) / sizeof(int);
    int i;
    int exitStatut = 1; // The page return code
    int userChoice = 0; // The id of current selected option
    int nbMaxUserChoice = contentIdSize;
    int isFirstLoop = 1;

    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 15);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 25);
    TTF_Font *font3 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 30);

    background1 = loadImage("data/img/background/arctis.jpg");
    cursor = loadImage("data/img/cursor/cursor.png");

    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos;
    SDL_Rect textPosDefault = { 250, 150, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};
    SDL_Rect pageTitlePos = {300, 50, 0, 0};
    SDL_Event event;

    SDL_FlushEvent(SDL_KEYDOWN);
    while ( isFirstLoop || SDL_WaitEvent(&event) ){
        isFirstLoop = 0;
        //--------------------------Events management-----------------------------------------------------------------------
        if(event.type == SDL_QUIT){ // Leave the game
            exitStatut = 0;
            break;
        }
        // Go back in game
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            exitStatut = 1000; // Keep playing
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){// Change userChoice
            userChoice -= 1;
        }else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN){// Change userChoice
            userChoice += 1;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){// Validate userChoice
            switch(userChoice){
                case 0:
                    exitStatut = 7; //Restart
                    break;
                case 1:
                    exitStatut = 1000; // Keep playing
                    break;
                case 2:
                    exitStatut = 2; // Go back to level selection
                    break;
                case 3:
                    exitStatut = 1;// Go back to main menu
                    break;
            }
            break;
        }

         // Set a limit to userChoice
        userChoice = userChoice % nbMaxUserChoice;
        if(userChoice < 0){
            userChoice = nbMaxUserChoice - 1;
        }

        textPos = textPosDefault;

        //--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);

        // Printing the page title
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[pageTitleId]->sentenceSize, font3, textColor, pageTitlePos);
        // Printing page content
        for( i = 0; i < contentIdSize; i++){
            if( i == userChoice){
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[contentId[i]]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[contentId[i]]->sentenceSize, font3, textColor, textPos);
                textPos.x -= cursor->w - 2;
                SDL_BlitSurface(cursor,NULL,SDL_GetWindowSurface(p_window),&textPos);
                textPos.x += cursor->w - 2;
            }else{
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[contentId[i]]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[contentId[i]]->sentenceSize, font2, textColor, textPos);
            }
            textPos.y += jumpLineGapSize;
        }
        // Indication to go back
        textPos.y = SDL_GetWindowSurface(p_window)->h - 120;
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[12]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[12]->sentenceSize, font1, textColor, textPos);

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }

    //-------------------------------- Free memory--------------------------------------------

    SDL_FreeSurface(background1);
    SDL_FreeSurface(cursor);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);

    return exitStatut;
}//--------------------------------------------------------------------------------------------------------------------
