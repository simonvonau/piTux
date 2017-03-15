#include "translationManager.h"



TranslationManager *initTranslationManager(char *path){
// Init a new TranslationManager
    TranslationManager *res = malloc(sizeof(TranslationManager));
    if(res == NULL){
        reportErreur("initTranslationManager():error malloc()");
    }
    res->currLangageId = 0;
    res->allTranslations = initTranslations(path, &(res->allTranslationsSize), &(res->nbLangages), &(res->langagesList));
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyTranslationManager(TranslationManager *currTranslationManager){
// Free TranslationManager memory
    int i;
    for( i = 0; i < currTranslationManager->allTranslationsSize; i++){
        destroyTranslation(currTranslationManager->allTranslations[i]);
    }
    free(currTranslationManager->allTranslations);

    destroyTranslation(currTranslationManager->langagesList);
    free(currTranslationManager);
}//--------------------------------------------------------------------------------------------------------------------

