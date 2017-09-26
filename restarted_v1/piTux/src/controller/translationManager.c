#include "translationManager.h"



TranslationManager *initTranslationManager(char *p_path){
// Init a new TranslationManager
    TranslationManager *res = malloc(sizeof(TranslationManager));
    if(res == NULL){
        reportErreur("initTranslationManager():error malloc()");
    }
    res->currLanguageId = 0;
    res->allTranslations = initTranslations(p_path, &(res->allTranslationsSize), &(res->nbLanguages), &(res->languagesList));
    return res;
}//--------------------------------------------------------------------------------------------------------------------

void destroyTranslationManager(TranslationManager *p_translaMgr){
// Free TranslationManager memory
    int i;
    for( i = 0; i < p_translaMgr->allTranslationsSize; i++){
        destroyTranslation(p_translaMgr->allTranslations[i]);
    }
    free(p_translaMgr->allTranslations);

    destroyTranslation(p_translaMgr->languagesList);
    free(p_translaMgr);
}//--------------------------------------------------------------------------------------------------------------------

