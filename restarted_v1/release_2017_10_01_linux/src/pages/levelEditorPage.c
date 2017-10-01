#include "levelEditorPage.h"
void fillToolBox(SDL_Window *p_window, int p_toolType, int p_startPosX, int p_startPosY, SDL_Surface *p_cursor, int p_selectedTool, int p_cursorDecalX, int p_cursorDecalY, GameManager *p_gameMgr){
// Display tools + cursor
    int i;
    SDL_Rect objectPos = { p_startPosX, p_startPosY, 0, 0};
    SDL_Rect cursorPos = { p_cursorDecalX, 0, 0, 0};
    switch(p_toolType){
        case 0: // Displaying Blocks
            for(i = 0; i < p_gameMgr->allBlocksSize; i++){
                SDL_BlitSurface(p_gameMgr->allBlocks[i]->sprites[0][0], NULL, SDL_GetWindowSurface(p_window), &objectPos);
                if(i == p_selectedTool){
                    cursorPos.y = objectPos.y + p_gameMgr->allBlocks[i]->refColl->height / 2;
                    cursorPos.y += p_cursorDecalY;
                    SDL_BlitSurface(p_cursor, NULL, SDL_GetWindowSurface(p_window), &cursorPos);
                }
                objectPos.y += p_gameMgr->allBlocks[i]->refColl->height;
            }
            break;
        case 1: // Displaying Bonus
            for(i = 0; i < p_gameMgr->allBonusSize; i++){
                SDL_BlitSurface(p_gameMgr->allBonus[i]->sprites[0][0], NULL, SDL_GetWindowSurface(p_window), &objectPos);
                if(i == p_selectedTool){
                    cursorPos.y = objectPos.y + p_gameMgr->allBonus[i]->refColl->height / 2;
                    cursorPos.y += p_cursorDecalY;
                    SDL_BlitSurface(p_cursor, NULL, SDL_GetWindowSurface(p_window), &cursorPos);
                }
                objectPos.y += p_gameMgr->allBonus[i]->refColl->height;
            }
            break;
        case 2: // Displaying Enemies
            for(i = 0; i < p_gameMgr->allEnemiesSize; i++){
                SDL_BlitSurface(p_gameMgr->allEnemies[i]->sprites[0][0], NULL, SDL_GetWindowSurface(p_window), &objectPos);
                if(i == p_selectedTool){
                    cursorPos.y = objectPos.y + p_gameMgr->allEnemies[i]->actionColl[0]->height / 2;
                    cursorPos.y += p_cursorDecalY;
                    SDL_BlitSurface(p_cursor, NULL, SDL_GetWindowSurface(p_window), &cursorPos);
                }
                objectPos.y += p_gameMgr->allEnemies[i]->actionColl[0]->height;
            }
            break;
    }

}//--------------------------------------------------------------------------------------------------------------------
void selectTool(int p_toolType, int p_toolBoxStartX, int p_toolBoxStartY, int p_clicX, int p_clicY, int *p_selectedTool, GameManager *p_gameMgr){
// Get selected tool Id
    int i;
    int heightSum = p_toolBoxStartY;

    if(p_clicX >= p_toolBoxStartX){
        switch(p_toolType){
        case 0: // Block
            for(i = 0; i < p_gameMgr->allBlocksSize; i++){
                if(p_clicY > heightSum && p_clicY <= heightSum + p_gameMgr->allBlocks[i]->refColl->height){
                    *p_selectedTool = i;
                    break;
                }
                heightSum += p_gameMgr->allBlocks[i]->refColl->height;
            }
            break;
        case 1: //Bonus
            for(i = 0; i < p_gameMgr->allBonusSize; i++){
                if(p_clicY > heightSum && p_clicY <= heightSum + p_gameMgr->allBonus[i]->refColl->height){
                    *p_selectedTool = i;
                    break;
                }
                heightSum += p_gameMgr->allBonus[i]->refColl->height;
            }
            break;
        case 2: //enemies
            for(i = 0; i < p_gameMgr->allEnemiesSize; i++){
                if(p_clicY > heightSum && p_clicY <= heightSum + p_gameMgr->allEnemies[i]->actionColl[0]->height){
                    *p_selectedTool = i;
                    break;
                }
                heightSum += p_gameMgr->allEnemies[i]->actionColl[0]->height;
            }
            break;


        }

    }

}//--------------------------------------------------------------------------------------------------------------------
int displayLevelEditorPage(SDL_Window *p_window, GameManager *p_gameMgr){
    SDL_Surface *cursor, *background1, *toolbox, *startLevelCursor, *endLevelCursor;
    TTF_Font *font1 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 25);
    TTF_Font *font2 = TTF_OpenFont("data/fonts/dejavu/DejaVuSans.ttf", 15);
    SDL_Event event;
    Button *leftArrow, *rightArrow;

    int clicX, clicY;
    int nextPage = 1;
    char levelCustomPath[] = "data/world1/level_custom.txt";
    int toolTypeId = 0; // To choose between enemy/bonus/block/
    int toolTypeIdMax = 3;
    int selectedToolId = 0;
    // To travel across the level
    int deplaX = 0;
    int deplaY = 0;
    int lastSaveDate = 0;
    int saveLaps = 2000; // Time before next auto saving
    int startTime = SDL_GetTicks();

    // Init level
    loadLevelByGameMgr(p_gameMgr, levelCustomPath, sizeof(levelCustomPath));

    background1 = loadImage("data/img/background/arctis.jpg");
    toolbox = loadImage("data/img/background/toolbox.jpg");
    cursor = loadImage("data/img/cursor/cursor2.png");
    leftArrow = initButton("data/img/gui/buttons/leftArrow.txt");
    rightArrow = initButton("data/img/gui/buttons/rightArrow.txt");
    startLevelCursor = loadImage("data/img/cursor/level_start.png");
    endLevelCursor = loadImage("data/img/cursor/level_end.png");

    // Loading ambiance music
    loadMusic(currMusicManager, "data/music/greatgigantic.ogg");
    playMusic(currMusicManager);

    SDL_Color textColor = {0, 0, 0};
    SDL_Rect textPos = { SDL_GetWindowSurface(p_window)->w - toolbox->w + leftArrow->guiWidth + 5,10, 0, 0};
    SDL_Rect escapeTextPos = { 0, SDL_GetWindowSurface(p_window)->h - 15, 0, 0};
    SDL_Rect nullPos = { 0, 0, 0, 0};

    SDL_Rect startLevelPosDefault = { p_gameMgr->levelManager->currLevel->herosStartX
    , SDL_GetWindowSurface(p_window)->h - p_gameMgr->levelManager->currLevel->herosStartY - startLevelCursor->h,0, 0};
    SDL_Rect endLevelPosDefault = { p_gameMgr->levelManager->currLevel->finishPosX, SDL_GetWindowSurface(p_window)->h - endLevelCursor->h - 96, 0, 0};
    SDL_Rect startLevelPosCurr = { 0, 0, 0, 0};
    SDL_Rect endLevelPosCurr = { 0, 0, 0, 0};
    SDL_Rect toolboxPos = { SDL_GetWindowSurface(p_window)->w - toolbox->w, 0, 0, 0};

    leftArrow->posX = SDL_GetWindowSurface(p_window)->w - toolbox->w;
    leftArrow->posY = 0;
    rightArrow->posX = SDL_GetWindowSurface(p_window)->w - leftArrow->guiWidth;
    rightArrow->posY = 0;

    while ( SDL_WaitEvent(&event) ){
//--------------------------Events management-----------------------------------------------------------------------
        if(event.type == SDL_QUIT){
            nextPage = 0;// Leave the game
            break;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
            nextPage = 1; // Go back to main menu
            break;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&clicX, &clicY);
            // Test buttons pressed existence:
            if( isClicked(leftArrow, clicX, clicY)){
                toolTypeId -=1;
                selectedToolId = 0;
            }
            if( isClicked(rightArrow, clicX, clicY)){
                toolTypeId +=1;
                selectedToolId = 0;
            }
            // If you select a other object to place on the level
            selectTool(toolTypeId, SDL_GetWindowSurface(p_window)->w - toolbox->w, leftArrow->posY + leftArrow->guiHeight
                       , clicX, clicY, &selectedToolId, p_gameMgr);
            // Add a element to level
            if(clicX < SDL_GetWindowSurface(p_window)->w - toolbox->w ){
                addElementToLevelByGameMgr(p_gameMgr, toolTypeId, selectedToolId, clicX + deplaX,
                                  SDL_GetWindowSurface(p_window)->h - clicY + deplaY);
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
            SDL_GetMouseState(&clicX, &clicY);
            // Remove element(s) from level
            if(clicX < SDL_GetWindowSurface(p_window)->w - toolbox->w ){
                removeElementFromLevel(p_gameMgr->levelManager->currLevel, clicX + deplaX, SDL_GetWindowSurface(p_window)->h - clicY + deplaY);
            }
        }
        // Traveling across the level
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT){
            deplaX += 50;
        }else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT){
            deplaX -= 50;
        }
        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){
            deplaY += 50;
        }else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN){
            deplaY -= 50;
        }
        // Open help page
        if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_F1){
            if ( !displayHelpSubPage(p_window, p_gameMgr)){
                nextPage = 0; // Leave the game
                break;
            }
        }
//------------------------------------management--------------------------------------------------------------------------------------

        // Which type of tools: block/enemy/bonus/...
        if( toolTypeId < 0){toolTypeId = toolTypeIdMax - 1;}
        toolTypeId = toolTypeId % toolTypeIdMax;

        startLevelPosCurr.x = startLevelPosDefault.x - deplaX;
        startLevelPosCurr.y = startLevelPosDefault.y + deplaY;
        endLevelPosCurr.x = endLevelPosDefault.x - deplaX;
        endLevelPosCurr.y = endLevelPosDefault.y + deplaY;

        // Auto saving after a timelapse
        startTime = SDL_GetTicks();
        if (startTime - lastSaveDate >= saveLaps){
            lastSaveDate = startTime;
            saveLevelByManager(p_gameMgr->levelManager);
        }

//--------------------------------Laying out ---------------------------------------------------------------------------------------
        SDL_BlitSurface(background1, NULL, SDL_GetWindowSurface(p_window), &nullPos);
        displayLevelByLevelManager(p_gameMgr->levelManager->currLevel, p_window, 0, deplaX, deplaY
                     ,p_gameMgr->allBlocks, p_gameMgr->allBonus, p_gameMgr->allEnemies, p_gameMgr->fireBullet);
        // Press escape to quit message
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[12]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[12]->sentenceSize, font2, textColor, escapeTextPos);
        // Help message
        setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[28]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[28]->sentenceSize, font2, textColor, nullPos);
        // Displaying start and end of current level
        SDL_BlitSurface(startLevelCursor, NULL, SDL_GetWindowSurface(p_window), &startLevelPosCurr);
        SDL_BlitSurface(endLevelCursor, NULL, SDL_GetWindowSurface(p_window), &endLevelPosCurr);

        // ToolBox background
        SDL_BlitSurface(toolbox, NULL, SDL_GetWindowSurface(p_window), &toolboxPos);
        // Printing the toolBox content type (block,enemy or bonus)
        switch(toolTypeId){
            case 0:
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[30]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[30]->sentenceSize, font1, textColor, textPos);
                break;
            case 1:
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[31]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[31]->sentenceSize, font1, textColor, textPos);
                break;
            case 2:
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[32]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[32]->sentenceSize, font1, textColor, textPos);
                break;
            default:
                setTextLayout(p_window,
                        p_gameMgr->translaManager->allTranslations[30]->sentence[p_gameMgr->translaManager->currLanguageId],
                        p_gameMgr->translaManager->allTranslations[30]->sentenceSize, font1, textColor, textPos);
                toolTypeId = 0;
                break;
        }

        // 2 buttons for object type changing
        displayButton(p_window, leftArrow);
        displayButton(p_window, rightArrow);
        // Filling the toolbox
        fillToolBox(p_window, toolTypeId, leftArrow->posX + leftArrow->guiWidth, leftArrow->posY + leftArrow->guiHeight
                    , cursor, selectedToolId, leftArrow->posX, - cursor->h / 2, p_gameMgr);

        // Window refreshing
        SDL_UpdateWindowSurface(p_window);
    }

    // Saving before leaving
    saveLevelByManager(p_gameMgr->levelManager);
//-------------------------------- Free memory--------------------------------------------
    destroyButton(leftArrow);
    destroyButton(rightArrow);
    SDL_FreeSurface(background1);
    SDL_FreeSurface(toolbox);
    SDL_FreeSurface(cursor);
    SDL_FreeSurface(startLevelCursor);
    SDL_FreeSurface(endLevelCursor);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    return nextPage;
}//--------------------------------------------------------------------------------------------------------------------
