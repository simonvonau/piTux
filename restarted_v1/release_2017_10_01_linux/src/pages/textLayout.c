#include "textLayout.h"


void setTextLayout(SDL_Window* p_window, char *p_text,int p_textSize,TTF_Font* p_font,SDL_Color p_textColor,SDL_Rect p_textPos){
// Display text
    char endLine = '$';
    int jumpSize = 30;// Size between two lines
    int i;
    SDL_Surface *label;
    char  **buff;
    int buffSize1;
    // Declared in bottom : int buffSize2[nbMaxElemPerLine];
    int nbLines = 0;

    // Line accounting
    for(i = 0; i < p_textSize && p_text[i] != '\0'; i++){
        if(p_text[i] == endLine){nbLines += 1;}
    }
    if(p_text[p_textSize-1] != endLine){nbLines += 1;}
    int buffSize2[nbLines];


    splitString(p_text, p_textSize, '$', &buff, &buffSize1, buffSize2, nbLines, LINE_SIZE_MAX);

    // Print each line
    for(i = 0; i < buffSize1; i++){
        label = TTF_RenderText_Blended(p_font, buff[i], p_textColor);
        SDL_BlitSurface(label, NULL, SDL_GetWindowSurface(p_window), &p_textPos);
        SDL_FreeSurface(label);

        p_textPos.y += jumpSize;
    }
    for(i = 0; i < buffSize1; i++){
        free(buff[i]);
    }
    free(buff);
}//--------------------------------------------------------------------------------------------------------------------

char *setTimeLayout(int p_time, int p_wishedSize){
// Put time from millisecondes to char
    int maxSize = 200;
    if(p_wishedSize <= 0 || p_wishedSize > maxSize){return "";}
    char tempArray[maxSize];
    char * res;
    sprintf(tempArray, "%d", p_time / 1000);

    res = malloc(p_wishedSize * sizeof(char));
    if(res == NULL){reportErreur("Error page/setTimeLayout() malloc() failed");}

    memcpy(res, tempArray, p_wishedSize * sizeof(char));
    return res;
}


