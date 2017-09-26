#include "button.h"

Button *initButton(char *p_path){
// Load a button from a file
    FILE *file = fopen ( p_path, "r" );
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
        perror ( p_path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);
    return res;
}//------------------------------------------------------------------------------------------------------------------------

void displayButton(SDL_Window *p_window, Button *p_button){
// Display a specified button
    SDL_Rect buttPos = { 0, 0, 0, 0};
    buttPos.x = p_button->posX;
    buttPos.y = p_button->posY;
    SDL_BlitSurface(p_button->background, NULL, SDL_GetWindowSurface(p_window), &buttPos);
}//------------------------------------------------------------------------------------------------------------------------

int isClicked(Button *p_button, int p_clicX, int p_clicY){
// Is a button clicked or not
    if(p_button->isEnabled){
        if(p_clicX > p_button->posX && p_clicX < p_button->posX + p_button->guiWidth){
            if(p_clicY > p_button->posY && p_clicY < p_button->posY + p_button->guiHeight){
                return 1;
            }
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

void destroyButton(Button *p_button){
// Free a Button
    SDL_FreeSurface(p_button->background);
    free(p_button);
}//------------------------------------------------------------------------------------------------------------------------
