#include "game.h"

Game* initGame(char *path){
// Init a game
    Game * res = malloc(sizeof(Game));
    if(res == NULL ){
        reportErreur("Error malloc initGame()");
    }

    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 2;
    char  **buff;
    char line[LINE_SIZE_MAX];
    int i;
    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', LINE_SIZE_MAX, nbMaxElemPerLine, LINE_SIZE_MAX);
            if(strcmp(buff[0], "[Header]") == 0){

            }else if(strcmp(buff[0], "[AllTranslations]") == 0){
                res->allTranslationsFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allTranslationsFile == NULL){reportErreur("initGame():error malloc(...)0");}
                strncpy(res->allTranslationsFile, buff[1], LINE_SIZE_MAX);
            }else if(strcmp(buff[0], "[AllBlocks]") == 0){
                res->allBlocksFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allBlocksFile == NULL){reportErreur("initGame():error malloc(...)1");}
                strncpy(res->allBlocksFile, buff[1], LINE_SIZE_MAX);
            }else if(strcmp(buff[0], "[AllEnemies]") == 0){
                res->allEnemiesFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allEnemiesFile == NULL){reportErreur("initGame():error malloc(...)2");}
                strncpy(res->allEnemiesFile, buff[1], LINE_SIZE_MAX);
            }else if(strcmp(buff[0], "[Heros]") == 0){
                res->allHerosFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allHerosFile == NULL){reportErreur("initGame():error malloc(...)3");}
                strncpy(res->allHerosFile, buff[1], LINE_SIZE_MAX);
            }else if(strcmp(buff[0], "[AllBonus]") == 0){
                res->allBonusFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allBonusFile == NULL){reportErreur("initGame():error malloc(...)4");}
                strncpy(res->allBonusFile, buff[1], LINE_SIZE_MAX);
            }else{
                reportErreur("Cannot read a line in the main game setings file!");
            }
        }
        fclose ( file );
    }else{
        perror ( path );
    }
    // Freeing buffer memory
    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);


    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyGame(Game *currGame){
// Free a game memory
    free(currGame->allTranslationsFile);
    free(currGame->allBlocksFile);
    free(currGame->allEnemiesFile);
    free(currGame->allBonusFile);
    free(currGame->allHerosFile);
    free(currGame);
}//--------------------------------------------------------------------------------------------------------------------

