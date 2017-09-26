#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include "../utils/reporting.h"
#include "../utils/arrayProcessing.h"

typedef struct{
    char **sentence;
    int nbLanguages;
    int sentenceSize;

}Translation;

Translation ** initTranslations(char *p_path, int *p_resSize, int *p_nbLanguages, Translation **p_languagesList);
Translation* initTranslation(int p_nbLanguages, int p_sentenceSize, int p_startIndex, char ** p_sentence);
void destroyTranslation(Translation *p_translation);

#endif
