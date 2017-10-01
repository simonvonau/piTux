#ifndef __TRANSLATIONMANAGER_H__
#define __TRANSLATIONMANAGER_H__
#include <stdlib.h>
#include <stdio.h>


#include "../utils/reporting.h"

#include "../model/translation.h"



typedef struct{
    Translation **allTranslations;
    int allTranslationsSize;
    int nbLanguages;
    Translation *languagesList; // One translation like: English,Français,Deutsch,Italiano
    int currLanguageId; // The current selected language
}TranslationManager;

TranslationManager *initTranslationManager(char *p_path);
void destroyTranslationManager(TranslationManager *p_translaMgr);




#endif
