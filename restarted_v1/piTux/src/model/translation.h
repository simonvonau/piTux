#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include "../utils/reporting.h"
#include "../utils/arrayProcessing.h"

typedef struct{
    char **sentence;
    int nbLangages;
    int sentenceSize;

}Translation;

Translation ** initTranslations(char *path, int *resSize, int *nbLangages, Translation **langagesList);
Translation* initTranslation(int currNbLangages, int currSentenceSize, int startIndex, char ** currSentence);
void destroyTranslation(Translation *currTranslation);

#endif
