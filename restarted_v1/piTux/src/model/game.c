#include "game.h"

Game* initGame(char *p_path){
// Init a game
    Game * res = malloc(sizeof(Game));
    if(res == NULL ){
        reportErreur("Error malloc initGame()");
    }

    FILE *file = fopen ( p_path, "r" );
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
            }else if(strcmp(buff[0], "[AllBullet]") == 0){
                res->fireBulletFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->fireBulletFile == NULL){reportErreur("initGame():error malloc(...)5");}
                strncpy(res->fireBulletFile, buff[1], LINE_SIZE_MAX);
            }else if(strcmp(buff[0], "[AllLevelIcons]") == 0){
                res->allLevelIconsFile = malloc(sizeof(char) * LINE_SIZE_MAX);
                if(res->allLevelIconsFile == NULL){reportErreur("initGame():error malloc(...)6");}
                strncpy(res->allLevelIconsFile, buff[1], LINE_SIZE_MAX);
            }else{
                reportErreur("Cannot read a line in the main game settings file!");
            }
        }
        fclose ( file );
    }else{
        perror ( p_path );
    }
    // Freeing buffer memory
    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);


    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyGame(Game *p_game){
// Free a game memory
    free(p_game->allTranslationsFile);
    free(p_game->allBlocksFile);
    free(p_game->allEnemiesFile);
    free(p_game->allBonusFile);
    free(p_game->allHerosFile);
    free(p_game);
}//--------------------------------------------------------------------------------------------------------------------

