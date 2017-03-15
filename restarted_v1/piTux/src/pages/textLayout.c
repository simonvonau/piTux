#include "textLayout.h"


void setTextLayout(SDL_Window* pWindow,char *text,int textSize,TTF_Font* font,SDL_Color textColor,SDL_Rect textPos){
// Display text
    char endLine = '$';
    int jumpSize = 30;// Size between two lines
    int maxLineWidth = 128;
    int i, j;
    SDL_Surface *label;
    int lastJumpCharIndex =- 1;// Id of the last end Line
    char **buff;
    int nbLines = 0;

    // Line accounting
    for(i = 0;i<textSize && text[i]!='\0'; i++){
        if(text[i] == endLine){nbLines += 1;}
    }
    if(text[textSize-1] != endLine){nbLines += 1;}

    buff =splitString(text, endLine, textSize, nbLines, maxLineWidth);

    // Print each line
    for(i=0; i < nbLines; i++){
        label = TTF_RenderText_Blended(font, buff[i], textColor);
        SDL_BlitSurface(label, NULL, SDL_GetWindowSurface(pWindow), &textPos);
        SDL_FreeSurface(label);

        textPos.y += jumpSize;
    }
    for(i = 0; i < nbLines; i++){
        free(buff[i]);
    }
    free(buff);
}//--------------------------------------------------------------------------------------------------------------------

char *setTimeLayout(int time, int wishedSize){
// Put time from millisecondes to char
    int maxSize = 200;
    char res[maxSize];
    int i;
    sprintf(res, "%d", time / 1000);

    // Put an end of line char
    if(wishedSize < 0){
        res[0] = '\0';
    }else if(wishedSize >= maxSize){
        res[maxSize - 1] = '\0';
    }else{
        res[wishedSize] = '\0';
    }
    return res;
}


