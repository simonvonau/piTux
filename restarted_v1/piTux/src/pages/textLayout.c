#include "textLayout.h"


void setTextLayout(SDL_Window* p_window, char *p_text,int p_textSize,TTF_Font* p_font,SDL_Color p_textColor,SDL_Rect p_textPos){
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
    for(i = 0;i<p_textSize && p_text[i]!='\0'; i++){
        if(p_text[i] == endLine){nbLines += 1;}
    }
    if(p_text[p_textSize-1] != endLine){nbLines += 1;}

    buff =splitString(p_text, endLine, p_textSize, nbLines, maxLineWidth);

    // Print each line
    for(i=0; i < nbLines; i++){
        label = TTF_RenderText_Blended(p_font, buff[i], p_textColor);
        SDL_BlitSurface(label, NULL, SDL_GetWindowSurface(p_window), &p_textPos);
        SDL_FreeSurface(label);

        p_textPos.y += jumpSize;
    }
    for(i = 0; i < nbLines; i++){
        free(buff[i]);
    }
    free(buff);
}//--------------------------------------------------------------------------------------------------------------------

char *setTimeLayout(int p_time, int p_wishedSize){
// Put time from millisecondes to char
    int maxSize = 200;
    char res[maxSize];
    int i;
    sprintf(res, "%d", p_time / 1000);

    // Put an end of line char
    if(p_wishedSize < 0){
        res[0] = '\0';
    }else if(p_wishedSize >= maxSize){
        res[maxSize - 1] = '\0';
    }else{
        res[p_wishedSize] = '\0';
    }
    return res;
}


