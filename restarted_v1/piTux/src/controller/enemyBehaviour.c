#include "enemyBehaviour.h"

void updateEnemyBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Chooses the right behaviour matching with currEnemyInstance
    switch(currEnemyInstance->idEnemy){
        case 0:
            updateMrFluffyBehaviour(currEnemyInstance, currEnemy, currCollMgr, loopTime);
            break;
        case 1:
            updateMrBombBehaviour(currEnemyInstance, currEnemy, currCollMgr, loopTime);
            break;
        case 2:
            updateMrIceBlockBehaviour(currEnemyInstance, currEnemy, currCollMgr, loopTime);
            break;
        default:
            reportErreur("Cannot update this enemy Id!");
            break;
    }
}//------------------------------------------------------------------------------------------------------------------------

void updateMrFluffyBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Update Mr Fluffy behaviour
    int movementX, movementY;
    int i;
    Collider **contacts;
    int contactsSize;

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);
    //printf("enemyBehaviour.c val: %d \n", contactsSize);
    for (i = 0; i < contactsSize; i++){

        // Testing if vertical collision
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            //printf("vertical collision \n");
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        }else{ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 1;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                currEnemyInstance->direction = 1;
            }else{ // If right touching
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 0;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                currEnemyInstance->direction = -1;
            }
        }


    }

    // Update Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;
    //printf("pos mr_fluffy %d, %d \n",currEnemyInstance->posX,currEnemyInstance->posY);
}//------------------------------------------------------------------------------------------------------------------------

void updateMrBombBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Update Mr Bomb behaviour
    int movementX, movementY;
    int i;
    Collider **contacts;
    int contactsSize;

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);
    //printf("enemyBehaviour.c val: %d \n", contactsSize);
    for (i = 0; i < contactsSize; i++){

        // Testing if vertical collision
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            //printf("vertical collision \n");
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        }else{ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 1;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                currEnemyInstance->direction = 1;
            }else{ // If right touching
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 0;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                currEnemyInstance->direction = -1;
            }
        }


    }

    // Update Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;

}//------------------------------------------------------------------------------------------------------------------------

void updateMrIceBlockBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Update Mr IceBlock behaviour
    int movementX, movementY;
    int i;
    Collider **contacts;
    int contactsSize;

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);
    //printf("enemyBehaviour.c val: %d \n", contactsSize);
    for (i = 0; i < contactsSize; i++){

        // Testing if vertical collision
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            //printf("vertical collision \n");
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        }else{ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 1;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                currEnemyInstance->direction = 1;
            }else{ // If right touching
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->currentActionId = 0;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 1;

                currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                currEnemyInstance->direction = -1;
            }
        }


    }

    // Update Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------


