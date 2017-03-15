#ifndef __TRANSLATIONMANAGER_H__
#define __TRANSLATIONMANAGER_H__
#include <stdlib.h>
#include <stdio.h>


#include "../utils/reporting.h"

#include "../controller/translationManager.h"
#include "../model/translation.h"



typedef struct{
    Translation **allTranslations;
    int allTranslationsSize;
    int nbLangages;
    Translation *langagesList; // One translation like: Anglais,Français,Deutsch,Italiano
    int currLangageId; // The current selected language
}TranslationManager;

TranslationManager *initTranslationManager(char *path);
void destroyTranslationManager(TranslationManager *currTranslationManager);




#endif
