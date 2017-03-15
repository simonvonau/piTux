#include "level.h"


Level *initLevel(char *path, int pathSize){
    FILE *file = fopen ( path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff = malloc(nbMaxElemPerLine * sizeof(char *));
    char line[lineSizeMax];
    int i;
    Level *res = malloc( sizeof(Level));

    strncpy(res->path, path, pathSize * sizeof(char));

    if ( file != NULL ){
        while ( fgets ( line, sizeof(line), file ) != NULL){
            buff = splitString(line, ';', lineSizeMax, nbMaxElemPerLine, lineSizeMax);
            if(strcmp(buff[0], "[Header]") == 0){
                res->blockInstancesSize = atoi(buff[3]);
                res->enemyInstancesSize = atoi(buff[4]);
                res->bonusInstancesSize = atoi(buff[5]);
                res->herosStartX = atoi(buff[6]);
                res->herosStartY = atoi(buff[7]);
                res->finishPosX = atoi(buff[8]);
                res->timeLimit = atoi(buff[9]);
                res->timeLeft = res->timeLimit;

                // Blocks Allocation
                res->blockInstances = malloc(res->blockInstancesSize * sizeof(BlockInstance *));
                if(res->blockInstances == NULL){
                    res->blockInstancesSize = 0;
                    reportErreur("Error malloc initLevel()");
                }
                // Bonus Allocation
                res->bonusInstances = malloc(res->bonusInstancesSize * sizeof(BonusInstance *));
                if(res->bonusInstances == NULL){
                    res->bonusInstancesSize = 0;
                    reportErreur("Error malloc initLevel()2");
                }
                // Enemies Allocation
                res->enemyInstances = malloc(res->enemyInstancesSize * sizeof(EnemyInstance *));
                if(res->enemyInstances == NULL){
                    res->bonusInstancesSize = 0;
                    reportErreur("Error malloc initLevel()3");
                }
            }else if(strcmp(buff[0], "[Block]") == 0){
                res->blockInstances[atoi(buff[1])] = initBlockInstance(atoi(buff[2]), atoi(buff[5]), atoi(buff[3]), atoi(buff[4]), NULL);
            }else if(strcmp(buff[0], "[Enemy]") == 0){
                res->enemyInstances[atoi(buff[1])] = initEnemyInstance(atoi(buff[2]), atoi(buff[3]), atoi(buff[4]), NULL, 0);
            }else if(strcmp(buff[0], "[Bonus]") == 0){
                res->bonusInstances[atoi(buff[1])] = initBonusInstance(atoi(buff[2]), atoi(buff[3]), atoi(buff[4]), NULL);
            }
        }

        fclose ( file );
    }else{
        perror ( path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);

    return res;
}//------------------------------------------------------------------------------------------------------------------------
void saveLevel(Level *lev){
// Saving the level
    char *line="==========================================================";
    FILE *file = fopen (lev->path, "w");
    int i;
    if ( file != NULL ){
        // Saving file header
        fprintf(file,"[Header];World 1;Level custom;%d;%d;%d;%d;%d;%d;%d;\n"
            ,lev->blockInstancesSize
            ,lev->enemyInstancesSize
            ,lev->bonusInstancesSize
            ,lev->herosStartX
            ,lev->herosStartY
            ,lev->finishPosX
            ,lev->timeLimit);
        fprintf(file,"%s\n",line);
        // Saving blockInstances
        for(i = 0; i < lev->blockInstancesSize; i++){
            fprintf(file,"[Block];%d;%d;%d;%d;%d;\n"
            ,i
            ,lev->blockInstances[i]->idBlock
            ,lev->blockInstances[i]->posX
            ,lev->blockInstances[i]->posY
            ,lev->blockInstances[i]->idBonus);
        }
        fprintf(file,"%s\n",line);
        // Saving EnemyInstances
        for(i = 0; i < lev->enemyInstancesSize; i++){
            fprintf(file,"[Enemy];%d;%d;%d;%d;\n"
            ,i
            ,lev->enemyInstances[i]->idEnemy
            ,lev->enemyInstances[i]->posX
            ,lev->enemyInstances[i]->posY);
        }
        fprintf(file,"%s\n",line);
        // Saving BonusInstances
        for(i = 0; i < lev->bonusInstancesSize; i++){
            fprintf(file,"[Bonus];%d;%d;%d;%d;\n"
            ,i
            ,lev->bonusInstances[i]->idBonus
            ,lev->bonusInstances[i]->posX
            ,lev->bonusInstances[i]->posY);
        }

        fclose ( file );
    }else{
        perror (lev->path);
    }
}//------------------------------------------------------------------------------------------------------------------------

void addBlockInstanceToLevel(Level *lev, int idBlock, int posX, int posY, Collider *coll){
// Add a new blockInstance to current level
    BlockInstance *temp[lev->blockInstancesSize];

    memcpy(temp, lev->blockInstances, sizeof(BlockInstance*) * lev->blockInstancesSize);

    // NB: Realloc causes some bugs
    free(lev->blockInstances);
    lev->blockInstances = malloc(sizeof(BlockInstance*) * (lev->blockInstancesSize + 1));

    if (lev->blockInstances == NULL){
        lev->blockInstancesSize = 0;
        reportErreur("Error addBlockToInstance() realloc()");
    }
    memcpy(lev->blockInstances, temp, sizeof(BlockInstance*) * lev->blockInstancesSize);
    lev->blockInstances[lev->blockInstancesSize] = initBlockInstance(idBlock, -1, posX, posY, coll);
    lev->blockInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void addBonusInstanceToLevel(Level *lev, int idBonus, int posX, int posY, Collider *coll){
// Add a new bonusInstance to current level
    BonusInstance *temp[lev->bonusInstancesSize];

    memcpy(temp, lev->bonusInstances, sizeof(BonusInstance*) * lev->bonusInstancesSize);

    // NB: Realloc causes some bugs
    //realloc(lev->bonusInstances, sizeof(BonusInstance*) * (lev->bonusInstancesSize + 1));
    free(lev->bonusInstances);
    lev->bonusInstances = malloc(sizeof(BonusInstance*) * (lev->bonusInstancesSize + 1));

    if (lev->bonusInstances == NULL){
        lev->bonusInstancesSize = 0;
        reportErreur("Error addBonusToInstance() realloc()");
    }
    memcpy(lev->bonusInstances, temp, sizeof(BonusInstance*) * lev->bonusInstancesSize);
    lev->bonusInstances[lev->bonusInstancesSize] = initBonusInstance(idBonus, posX, posY, coll);
    lev->bonusInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void addEnemyInstanceToLevel(Level *lev, int idEnemy, int posX, int posY, Collider **coll, int collSize){
// Add a new enemyInstance to current level
    EnemyInstance *temp[lev->enemyInstancesSize];

    memcpy(temp, lev->enemyInstances, sizeof(EnemyInstance*) * lev->enemyInstancesSize);

    // NB: Realloc causes some bugs
    //realloc(lev->enemyInstances, sizeof(EnemyInstance*) * (lev->enemyInstancesSize + 1));
    free(lev->enemyInstances);
    lev->enemyInstances = malloc(sizeof(EnemyInstance *) * (lev->enemyInstancesSize + 1));

    if (lev->enemyInstances == NULL){
        lev->enemyInstancesSize = 0;
        reportErreur("Error addEnemyInstanceToLevel() realloc()");
    }
    memcpy(lev->enemyInstances, temp, sizeof(EnemyInstance*) * lev->enemyInstancesSize);
    lev->enemyInstances[lev->enemyInstancesSize] = initEnemyInstance(idEnemy, posX, posY, coll, collSize);
    lev->enemyInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void removeBlockInstanceToLevel(Level *lev, int blockIndex){
// Remove a blockInstance from level (by it's index)
    int i;

    if (blockIndex >= 0 && blockIndex < lev->blockInstancesSize){
        destroyBlockInstance(lev->blockInstances[blockIndex]);
        for(i = blockIndex; i < lev->blockInstancesSize - 1; i++){
            lev->blockInstances[i] = lev->blockInstances[i+1];
        }
        lev->blockInstancesSize -= 1;
        realloc(lev->blockInstances, sizeof(BlockInstance*) * lev->blockInstancesSize);

        if(lev->blockInstances == NULL){
            lev->blockInstancesSize = 0;
            reportErreur("Error removeBlockInstanceToLevel(...)");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void removeBonusInstanceToLevel(Level *lev, int bonusIndex){
// Remove a bonusInstance from level (by it's index)
    int i;
    if (bonusIndex >= 0 && bonusIndex < lev->bonusInstancesSize){
        destroyBonusInstance(lev->bonusInstances[bonusIndex]);
        for(i = bonusIndex; i < lev->bonusInstancesSize - 1; i++){
            lev->bonusInstances[i] = lev->bonusInstances[i+1];
        }
        lev->bonusInstancesSize -= 1;
        realloc(lev->bonusInstances, sizeof(BonusInstance*) * lev->bonusInstancesSize);

        if(lev->bonusInstances == NULL){
            lev->bonusInstancesSize = 0;
            reportErreur("Error removeBonusInstanceToLevel(...)");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void removeEnemyInstanceToLevel(Level *lev, int enemyIndex){
// Remove a EnemyInstance from level (by it's index)
    int i;

    if (enemyIndex >= 0 && enemyIndex < lev->enemyInstancesSize){
        destroyEnemyInstance(lev->enemyInstances[enemyIndex]);
        for(i = enemyIndex; i < lev->enemyInstancesSize - 1; i++){
            lev->enemyInstances[i] = lev->enemyInstances[i+1];
        }
        lev->enemyInstancesSize -= 1;
        realloc(lev->enemyInstances, sizeof(EnemyInstance*) * lev->enemyInstancesSize);

        if(lev->enemyInstances == NULL){
            lev->enemyInstancesSize = 0;
            reportErreur("Error removeEnemyInstanceToLevel(...)");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

int checkIfBlockInstanceExistHere(Level *lev, int posX, int posY){
// Check if a blockInstance already exists here
    int i;
    for(i = 0; i < lev->blockInstancesSize; i++){
        if(lev->blockInstances[i]->posX == posX && lev->blockInstances[i]->posY == posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

int checkIfBonusInstanceExistHere(Level *lev, int posX, int posY){
// Check if a bonusInstance already exists here
    int i;
    for(i = 0; i < lev->bonusInstancesSize; i++){
        if(lev->bonusInstances[i]->posX == posX && lev->bonusInstances[i]->posY == posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

int checkIfEnemyInstanceExistHere(Level *lev, int posX, int posY){
// Check if a enemyInstance already exists here
    int i;
    for(i = 0; i < lev->enemyInstancesSize; i++){
        if(lev->enemyInstances[i]->posX == posX && lev->enemyInstances[i]->posY == posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

void addBonusToBlock(Level *lev, int idBlockInstance, int idBonus){
// Put a bonus inside a block
    lev->blockInstances[idBlockInstance]->idBonus = idBonus;
}//------------------------------------------------------------------------------------------------------------------------

void destroyLevel(Level *currLevel){
    int i;

    // Destroying blocks
    for(i = 0; i < currLevel->blockInstancesSize; i++){
        destroyBlockInstance(currLevel->blockInstances[i]);
    }
    free(currLevel->blockInstances);

    // Destroying bonus
    for(i = 0; i < currLevel->bonusInstancesSize; i++){
        destroyBonusInstance(currLevel->bonusInstances[i]);
    }
    free(currLevel->bonusInstances);

    // Destroying Enemies
    for(i = 0; i < currLevel->enemyInstancesSize; i++){
        destroyEnemyInstance(currLevel->enemyInstances[i]);
    }
    free(currLevel->enemyInstances);

    free(currLevel);
}//------------------------------------------------------------------------------------------------------------------------


