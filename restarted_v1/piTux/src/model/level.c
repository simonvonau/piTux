#include "level.h"


Level *initLevel(char *p_path, int p_pathSize){
    FILE *file = fopen ( p_path, "r" );
    int nbMaxElemPerLine = 10;
    int lineSizeMax = 512;// Max size of a line from the opened file
    char  **buff = malloc(nbMaxElemPerLine * sizeof(char *));
    char line[lineSizeMax];
    int i;
    Level *res = malloc( sizeof(Level));

    strncpy(res->path, p_path, p_pathSize * sizeof(char));

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
                // FireBullet initialization
                res->fireBulletInstances = NULL;
                res->fireBulletInstancesSize = 0;

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
        perror ( p_path );
    }

    for(i=0;i < nbMaxElemPerLine;i++){
        free(buff[i]);
    }
    free(buff);

    return res;
}//------------------------------------------------------------------------------------------------------------------------
void saveLevel(Level *p_level){
// Saving the level
    char *line="==========================================================";
    FILE *file = fopen (p_level->path, "w");
    int i;
    if ( file != NULL ){
        // Saving file header
        fprintf(file,"[Header];World 1;Level custom;%d;%d;%d;%d;%d;%d;%d;\n"
            ,p_level->blockInstancesSize
            ,p_level->enemyInstancesSize
            ,p_level->bonusInstancesSize
            ,p_level->herosStartX
            ,p_level->herosStartY
            ,p_level->finishPosX
            ,p_level->timeLimit);
        fprintf(file,"%s\n",line);
        // Saving blockInstances
        for(i = 0; i < p_level->blockInstancesSize; i++){
            fprintf(file,"[Block];%d;%d;%d;%d;%d;\n"
            ,i
            ,p_level->blockInstances[i]->idBlock
            ,p_level->blockInstances[i]->posX
            ,p_level->blockInstances[i]->posY
            ,p_level->blockInstances[i]->idBonus);
        }
        fprintf(file,"%s\n",line);
        // Saving EnemyInstances
        for(i = 0; i < p_level->enemyInstancesSize; i++){
            fprintf(file,"[Enemy];%d;%d;%d;%d;\n"
            ,i
            ,p_level->enemyInstances[i]->idEnemy
            ,p_level->enemyInstances[i]->posX
            ,p_level->enemyInstances[i]->posY);
        }
        fprintf(file,"%s\n",line);
        // Saving BonusInstances
        for(i = 0; i < p_level->bonusInstancesSize; i++){
            fprintf(file,"[Bonus];%d;%d;%d;%d;\n"
            ,i
            ,p_level->bonusInstances[i]->idBonus
            ,p_level->bonusInstances[i]->posX
            ,p_level->bonusInstances[i]->posY);
        }

        fclose ( file );
    }else{
        perror (p_level->path);
    }
}//------------------------------------------------------------------------------------------------------------------------

void addBlockInstanceToLevel(Level *p_lev, int p_idBlock, int p_posX, int p_posY, Collider *p_coll){
// Add a new blockInstance to current level
    BlockInstance *temp[p_lev->blockInstancesSize];

    memcpy(temp, p_lev->blockInstances, sizeof(BlockInstance*) * p_lev->blockInstancesSize);

    // NB: Realloc causes some bugs
    free(p_lev->blockInstances);
    p_lev->blockInstances = malloc(sizeof(BlockInstance*) * (p_lev->blockInstancesSize + 1));

    if (p_lev->blockInstances == NULL){
        p_lev->blockInstancesSize = 0;
        reportErreur("Error addBlockToInstance() malloc()");
    }
    memcpy(p_lev->blockInstances, temp, sizeof(BlockInstance*) * p_lev->blockInstancesSize);
    p_lev->blockInstances[p_lev->blockInstancesSize] = initBlockInstance(p_idBlock, -1, p_posX, p_posY, p_coll);
    p_lev->blockInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void addBonusInstanceToLevel(Level *p_lev, int p_idBonus, int p_posX, int p_posY, Collider *p_coll){
// Add a new bonusInstance to current level
    BonusInstance *temp[p_lev->bonusInstancesSize];

    memcpy(temp, p_lev->bonusInstances, sizeof(BonusInstance*) * p_lev->bonusInstancesSize);

    // NB: Realloc causes some bugs
    //realloc(p_lev->bonusInstances, sizeof(BonusInstance*) * (p_lev->bonusInstancesSize + 1));
    free(p_lev->bonusInstances);
    p_lev->bonusInstances = malloc(sizeof(BonusInstance*) * (p_lev->bonusInstancesSize + 1));

    if (p_lev->bonusInstances == NULL){
        p_lev->bonusInstancesSize = 0;
        reportErreur("Error addBonusToInstance() malloc()");
    }
    memcpy(p_lev->bonusInstances, temp, sizeof(BonusInstance*) * p_lev->bonusInstancesSize);
    p_lev->bonusInstances[p_lev->bonusInstancesSize] = initBonusInstance(p_idBonus, p_posX, p_posY, p_coll);
    p_lev->bonusInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void addEnemyInstanceToLevel(Level *p_lev, int p_idEnemy, int p_posX, int p_posY, Collider **p_coll, int p_collSize){
// Add a new enemyInstance to current level
    EnemyInstance *temp[p_lev->enemyInstancesSize];

    memcpy(temp, p_lev->enemyInstances, sizeof(EnemyInstance*) * p_lev->enemyInstancesSize);

    // NB: Realloc causes some bugs
    //realloc(p_lev->enemyInstances, sizeof(EnemyInstance*) * (p_lev->enemyInstancesSize + 1));
    free(p_lev->enemyInstances);
    p_lev->enemyInstances = malloc(sizeof(EnemyInstance *) * (p_lev->enemyInstancesSize + 1));

    if (p_lev->enemyInstances == NULL){
        p_lev->enemyInstancesSize = 0;
        reportErreur("Error addEnemyInstanceToLevel() malloc()");
    }
    memcpy(p_lev->enemyInstances, temp, sizeof(EnemyInstance*) * p_lev->enemyInstancesSize);
    p_lev->enemyInstances[p_lev->enemyInstancesSize] = initEnemyInstance(p_idEnemy, p_posX, p_posY, p_coll, p_collSize);
    p_lev->enemyInstancesSize += 1;
}//------------------------------------------------------------------------------------------------------------------------

void addBulletInstanceToLevel(Level *p_lev, int p_posX, int p_posY, Collider *p_coll, int p_directionX, int p_directionY, int p_lifeTimeLeft){
// Add a BulletInstance to a level
    FireBulletInstance *tempArray[p_lev->fireBulletInstancesSize];
    if(p_lev->fireBulletInstancesSize == 0){
        p_lev->fireBulletInstances = malloc(sizeof(FireBulletInstance*));
    }else{
        memcpy(tempArray, p_lev->fireBulletInstances, p_lev->fireBulletInstancesSize * sizeof(FireBulletInstance*));
        free(p_lev->fireBulletInstances);
        p_lev->fireBulletInstances = malloc((p_lev->fireBulletInstancesSize + 1) * sizeof(FireBulletInstance*));
        memcpy(p_lev->fireBulletInstances, tempArray, p_lev->fireBulletInstancesSize * sizeof(FireBulletInstance*));
    }
    if(p_lev->fireBulletInstances == NULL){
        p_lev->fireBulletInstancesSize = 0;
        reportErreur("Error addBulletInstanceToLevel() malloc()");
    }
    p_lev->fireBulletInstancesSize += 1;
    p_lev->fireBulletInstances[p_lev->fireBulletInstancesSize - 1] = initFireBulletInstance(p_posX, p_posY, p_directionX, p_directionY, p_coll, p_lifeTimeLeft);
}//------------------------------------------------------------------------------------------------------------------------

void removeBlockInstanceToLevel(Level *p_lev, int p_blockIndex){
// Remove a blockInstance from level (by it's index)
    BlockInstance *tempBlock;
    BlockInstance *tempArray[p_lev->blockInstancesSize];

    if (p_blockIndex >= 0 && p_blockIndex < p_lev->blockInstancesSize){
        tempBlock = p_lev->blockInstances[p_blockIndex];
        p_lev->blockInstances[p_blockIndex] = p_lev->blockInstances[p_lev->blockInstancesSize - 1];
        p_lev->blockInstances[p_lev->blockInstancesSize - 1] = tempBlock;
        p_lev->blockInstancesSize -= 1;

        memcpy(tempArray, p_lev->blockInstances, sizeof(BlockInstance*) * (p_lev->blockInstancesSize + 1));
        free(p_lev->blockInstances);
        p_lev->blockInstances = malloc(sizeof(BlockInstance*) * p_lev->blockInstancesSize);
        memcpy(p_lev->blockInstances, tempArray, sizeof(BlockInstance*) * p_lev->blockInstancesSize);
        destroyBlockInstance(tempArray[p_lev->blockInstancesSize]);

        if(p_lev->blockInstances == NULL){
            p_lev->blockInstancesSize = 0;
            reportErreur("Error removeBlockInstanceToLevel(...)");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void removeBonusInstanceToLevel(Level *p_lev, int p_bonusIndex){
// Remove a bonusInstance from level (by it's index)
    BonusInstance *tempBonus;
    BonusInstance *temp[p_lev->bonusInstancesSize];

    if (p_bonusIndex >= 0 && p_bonusIndex < p_lev->bonusInstancesSize){
        tempBonus = p_lev->bonusInstances[p_bonusIndex];
        p_lev->bonusInstances[p_bonusIndex] = p_lev->bonusInstances[p_lev->bonusInstancesSize - 1];
        p_lev->bonusInstances[p_lev->bonusInstancesSize - 1] = tempBonus;
        p_lev->bonusInstancesSize -= 1;

        memcpy(temp, p_lev->bonusInstances, sizeof(BonusInstance*) * (p_lev->bonusInstancesSize + 1));
        free(p_lev->bonusInstances);
        p_lev->bonusInstances = malloc(sizeof(BonusInstance*) * p_lev->bonusInstancesSize);
        memcpy(p_lev->bonusInstances, temp, sizeof(BonusInstance*) * (p_lev->bonusInstancesSize));
        destroyBonusInstance(temp[p_lev->bonusInstancesSize]);

        if(p_lev->bonusInstances == NULL){
            p_lev->bonusInstancesSize = 0;
            reportErreur("Error removeBonusInstanceToLevel(...)");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void removeEnemyInstanceToLevel(Level *p_lev, int p_enemyIndex){
// Remove a EnemyInstance from level (by it's index)
    EnemyInstance *tempEne;
    EnemyInstance *tempArray[p_lev->enemyInstancesSize];

    if (p_enemyIndex >= 0 && p_enemyIndex < p_lev->enemyInstancesSize){
        tempEne = p_lev->enemyInstances[p_enemyIndex];
        p_lev->enemyInstances[p_enemyIndex] = p_lev->enemyInstances[p_lev->enemyInstancesSize - 1];
        p_lev->enemyInstances[p_lev->enemyInstancesSize - 1] = tempEne;
        p_lev->enemyInstancesSize -= 1;

        memcpy(tempArray, p_lev->enemyInstances, sizeof(EnemyInstance*) * (p_lev->enemyInstancesSize + 1));
        free(p_lev->enemyInstances);
        p_lev->enemyInstances = malloc(sizeof(EnemyInstance*) * p_lev->enemyInstancesSize);
        memcpy(p_lev->enemyInstances, tempArray, sizeof(EnemyInstance*) * p_lev->enemyInstancesSize);
        destroyEnemyInstance(tempArray[p_lev->enemyInstancesSize]);

        //*** Realloc doesn't work, weird...
        //realloc(p_lev->enemyInstances, sizeof(EnemyInstance*) * (p_lev->enemyInstancesSize - 1));

        if(p_lev->enemyInstances == NULL){
            p_lev->enemyInstancesSize = 0;
            reportErreur("Error removeEnemyInstanceToLevel(...) malloc()");
        }
    }
}//------------------------------------------------------------------------------------------------------------------------

void removeFireBulletInstanceToLevel(Level *p_lev, int p_fireBulletIndex){
// Remove a fire bullet from a level
    FireBulletInstance *tempFBI;
    FireBulletInstance *tempArray[p_lev->fireBulletInstancesSize];

    if (p_fireBulletIndex >= 0 && p_fireBulletIndex < p_lev->fireBulletInstancesSize){
        tempFBI = p_lev->fireBulletInstances[p_fireBulletIndex];
        p_lev->fireBulletInstances[p_fireBulletIndex] = p_lev->fireBulletInstances[p_lev->fireBulletInstancesSize - 1];
        p_lev->fireBulletInstances[p_lev->fireBulletInstancesSize - 1] = tempFBI;
        p_lev->fireBulletInstancesSize -= 1;

        memcpy(tempArray, p_lev->fireBulletInstances, sizeof(FireBulletInstance*) * (p_lev->fireBulletInstancesSize + 1));
        free(p_lev->fireBulletInstances);
        p_lev->fireBulletInstances = malloc(sizeof(FireBulletInstance*) * p_lev->fireBulletInstancesSize);
        memcpy(p_lev->fireBulletInstances, tempArray, sizeof(FireBulletInstance*) * p_lev->fireBulletInstancesSize);
        destroyFireBulletInstance(tempArray[p_lev->fireBulletInstancesSize]);

        if(p_lev->fireBulletInstances == NULL){
            p_lev->fireBulletInstancesSize = 0;
            reportErreur("Error removeFireBulletInstanceToLevel(...) malloc()");
        }
    }


}//------------------------------------------------------------------------------------------------------------------------

int checkIfBlockInstanceExistHere(Level *p_lev, int p_posX, int p_posY){
// Check if a blockInstance already exists here
    int i;
    for(i = 0; i < p_lev->blockInstancesSize; i++){
        if(p_lev->blockInstances[i]->posX == p_posX && p_lev->blockInstances[i]->posY == p_posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

int checkIfBonusInstanceExistHere(Level *p_lev, int p_posX, int p_posY){
// Check if a bonusInstance already exists here
    int i;
    for(i = 0; i < p_lev->bonusInstancesSize; i++){
        if(p_lev->bonusInstances[i]->posX == p_posX && p_lev->bonusInstances[i]->posY == p_posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

int checkIfEnemyInstanceExistHere(Level *p_lev, int p_posX, int p_posY){
// Check if a enemyInstance already exists here
    int i;
    for(i = 0; i < p_lev->enemyInstancesSize; i++){
        if(p_lev->enemyInstances[i]->posX == p_posX && p_lev->enemyInstances[i]->posY == p_posY){
            return 1;
        }
    }
    return 0;
}//------------------------------------------------------------------------------------------------------------------------

void addBonusToBlock(Level *p_lev, int p_idBlockInstance, int p_idBonus){
// Put a bonus inside a block
    p_lev->blockInstances[p_idBlockInstance]->idBonus = p_idBonus;
}//------------------------------------------------------------------------------------------------------------------------

void destroyLevel(Level *p_level){
    int i;

    // Destroying blocks
    for(i = 0; i < p_level->blockInstancesSize; i++){
        destroyBlockInstance(p_level->blockInstances[i]);
    }
    free(p_level->blockInstances);

    // Destroying bonus
    for(i = 0; i < p_level->bonusInstancesSize; i++){
        destroyBonusInstance(p_level->bonusInstances[i]);
    }
    free(p_level->bonusInstances);

    // Destroying Enemies
    for(i = 0; i < p_level->enemyInstancesSize; i++){
        destroyEnemyInstance(p_level->enemyInstances[i]);
    }
    free(p_level->enemyInstances);

    // Destroying bullet
    for(i = 0; i < p_level->fireBulletInstancesSize; i++){
        destroyFireBulletInstance(p_level->fireBulletInstances[i]);
    }
    free(p_level->fireBulletInstances);

    free(p_level);
}//------------------------------------------------------------------------------------------------------------------------


