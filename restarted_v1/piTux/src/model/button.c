#include "button.h"

Button *initButton(char *path){
// Load a button from a file
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 4;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff;
    char line[lineSizeMax];
    Button* res = malloc(sizeof(Button));
    int i;

    if(res == NULL){
        reportErreur("Error malloc initButton()");
    }

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);

            if(strcmp(buff[0], "[Header]") == 0){
                res->background = loadImage(buff[3]);
                res->isEnabled = 1;
                res->posX = 0;
                res->posY = 0;
                res->guiHeight = atoi(buff[1]);
                res->guiWidth = atoi(buff[2]);
            }
        }
        fclose ( file );
    }else{
        perror ( path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void displayButton(SDL_Window *p_window, Button *currBut){
// Display a specified button
    SDL_Rect buttPos = { 0, 0, 0, 0};
    buttPos.x = currBut->posX;
    buttPos.y = currBut->posY;
    SDL_BlitSurface(currBut->background, NULL, SDL_GetWindowSurface(p_window), &buttPos);
}//------------------------------------------------------------------------------------------------------------------------

int isClicked(Button *currButt, int clicX, int clicY){
// Is a button clicked or not
    if(currButt->isEnabled){
        if(clicX > currButt->posX && clicX < currButt->posX + currButt->guiWidth){
            if(clicY > currButt->posY && clicY < currButt->posY + currButt->guiHeight){
                return 1;
            }
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

void destroyButton(Button *currButton){
// Free a Button
    SDL_FreeSurface(currButton->background);
    free(currButton);
}//------------------------------------------------------------------------------------------------------------------------
