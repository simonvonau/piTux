#include "enemyBehaviour.h"

void updateEnemyBehaviourAfterCollisionDetection(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
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
            reportErreur("Cannot update this enemy Id! (doesn't exists)");
            break;
    }
}//------------------------------------------------------------------------------------------------------------------------

void updateMrFluffyBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Update Mr Fluffy behaviour
    int movementX, movementY;
    int i;
    Collider **contacts;
    int contactsSize;
    int enemyFallY; // The deepness the enemy go down on the ground (due to gravity)

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);

    for (i = 0; i < contactsSize; i++){
        enemyFallY = currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY - currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY;

        // Dead after touched an explosion
        if(contacts[i]->ownerTag == TAG_EXPLOSION || contacts[i]->ownerTag == TAG_BULL){
            currEnemyInstance->isDead = 1;
            currEnemyInstance->lifeTime = 1000;
            currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
        }

        // If the colliding object is under
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        // If the colliding object is above
        }else if(currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY + currEnemyInstance->coll[currEnemyInstance->currentActionId]->height <= contacts[i]->lastPosY + enemyFallY){
            if(!currEnemyInstance->isDead && contacts[i]->ownerTag == TAG_HEROS_TUX){
                if(currEnemyInstance->direction == -1){
                    changeEnemyAction(currEnemyInstance, 2, 0);
                }else{
                    changeEnemyAction(currEnemyInstance, 3, 0);
                }
                currEnemyInstance->isDead = 1;
                currEnemyInstance->lifeTime = 500;
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
                currEnemyInstance->isExposedToGravity = 0;
            }
        }else if(!currEnemyInstance->isDead){ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                changeEnemyAction(currEnemyInstance, 1, 0);
                currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                currEnemyInstance->direction = 1;
            }else{ // If right touching
                changeEnemyAction(currEnemyInstance, 0, 0);
                currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                currEnemyInstance->direction = -1;
            }
        }
    }

    // Update Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------

void updateMrBombBehaviour(EnemyInstance *currEnemyInstance, Enemy currEnemy, ColliderManager *currCollMgr, int loopTime){
// Update Mr Bomb behaviour
    int movementX, movementY;
    int i;
    Collider **contacts;
    int contactsSize;
    int enemyFallY; // The deepness the enemy go down on the ground (due to gravity)

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);

    for (i = 0; i < contactsSize; i++){
        enemyFallY = currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY - currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY;

        // Mr Bomb explodes when he touches an explosion
        if((contacts[i]->ownerTag == TAG_EXPLOSION || contacts[i]->ownerTag == TAG_BULL) && currEnemyInstance->currentActionId != 4){
            changeEnemyAction(currEnemyInstance, 4, 0);
            currEnemyInstance->coll[currEnemyInstance->currentActionId]->ownerTag = TAG_EXPLOSION;
            currEnemyInstance->isDead = 1;
            currEnemyInstance->isExposedToGravity = 0;
            // Get the explosion duration (spritesQty * spriteDuration)
            currEnemyInstance->lifeTime = currEnemy.spriteDuration[currEnemyInstance->currentActionId] * currEnemy.spritesSize2[currEnemyInstance->currentActionId];
        }

        // If the colliding object is under
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        // If the colliding object is above
        }else if(currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY + currEnemyInstance->coll[currEnemyInstance->currentActionId]->height <= contacts[i]->lastPosY + enemyFallY){
            if(!currEnemyInstance->isDead && contacts[i]->ownerTag == TAG_HEROS_TUX){
                if(currEnemyInstance->direction == -1){
                    changeEnemyAction(currEnemyInstance, 2, 0);
                }else{
                    changeEnemyAction(currEnemyInstance, 3, 0);
                }
                currEnemyInstance->lifeTime = currEnemy.spriteDuration[currEnemyInstance->currentActionId] * currEnemy.spritesSize2[currEnemyInstance->currentActionId] + 200;
                currEnemyInstance->isDead = 1;
            }
        }else if(!currEnemyInstance->isDead){ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                changeEnemyAction(currEnemyInstance, 1, 0);
                currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                currEnemyInstance->direction = 1;
            }else{ // If right touching
                changeEnemyAction(currEnemyInstance, 0, 0);
                currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                currEnemyInstance->direction = -1;
            }
        }
    }

    // Create the explosion
    if((currEnemyInstance->currentActionId == 2 || currEnemyInstance->currentActionId == 3)
       && currEnemyInstance->currentSpriteId == currEnemy.spritesSize2[currEnemyInstance->currentActionId] - 1){
        changeEnemyAction(currEnemyInstance, 4, 0);
        currEnemyInstance->coll[currEnemyInstance->currentActionId]->ownerTag = TAG_EXPLOSION;
        currEnemyInstance->isDead = 1;
        currEnemyInstance->isExposedToGravity = 0;
        // Get the explosion duration (spritesQty * spriteDuration)
        currEnemyInstance->lifeTime = currEnemy.spriteDuration[currEnemyInstance->currentActionId] * currEnemy.spritesSize2[currEnemyInstance->currentActionId];
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
    int enemyFallY; // The deepness the enemy go down on the ground (due to gravity)

    // Collision detection
    getColliderTouching(currCollMgr, currEnemyInstance->coll[currEnemyInstance->currentActionId]->id, &contacts, &contactsSize);

    for (i = 0; i < contactsSize; i++){
        enemyFallY = currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY - currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY;

        // Melt after being touched by an explosion
        if(contacts[i]->ownerTag == TAG_EXPLOSION){
            if(currEnemyInstance->direction == -1){
                changeEnemyAction(currEnemyInstance, 2, 0);
            }else{
                changeEnemyAction(currEnemyInstance, 3, 0);
            }
            currEnemyInstance->isDead = 1;
            currEnemyInstance->lifeTime = currEnemy.spriteDuration[currEnemyInstance->currentActionId] * (currEnemy.spritesSize2[currEnemyInstance->currentActionId] - 1);
            currEnemyInstance->coll[currEnemyInstance->currentActionId]->isEnabled = 0;
            currEnemyInstance->isExposedToGravity = 0;
        }

        // If the colliding object is under
        if( currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
            currEnemyInstance->posY = contacts[i]->posY + contacts[i]->height;
        // If the colliding object is above
        }else if(currEnemyInstance->coll[currEnemyInstance->currentActionId]->lastPosY + currEnemyInstance->coll[currEnemyInstance->currentActionId]->height <= contacts[i]->lastPosY + enemyFallY){
            if(!currEnemyInstance->isDead && contacts[i]->ownerTag == TAG_HEROS_TUX){
                if(currEnemyInstance->direction == -1){
                    changeEnemyAction(currEnemyInstance, 4, 0);
                }else{
                    changeEnemyAction(currEnemyInstance, 5, 0);
                }
                currEnemyInstance->coll[currEnemyInstance->currentActionId]->ownerTag = TAG_DEAD_ENEMY;
                currEnemyInstance->isStuck = 1;
            }
        }else if(!currEnemyInstance->isDead){ // Lateral collision
            // If left touching
            if (currEnemyInstance->posX > contacts[i]->posX){
                if(contacts[i]->ownerTag != TAG_HEROS_TUX){
                    if(currEnemyInstance->currentActionId != 4 && currEnemyInstance->currentActionId != 5){
                        changeEnemyAction(currEnemyInstance, 1, 0);
                    }
                }else{
                    if(currEnemyInstance->isStuck){
                        currEnemyInstance->isStuck = 0;
                        changeEnemyAction(currEnemyInstance, 5, 0);
                        currEnemyInstance->coll[currEnemyInstance->currentActionId]->ownerTag = TAG_BULL;
                    }
                }
                if( (currEnemyInstance->currentActionId == 4 || currEnemyInstance->currentActionId == 5)
                && contacts[i]->ownerTag == TAG_ENEMY_MRBOMB || contacts[i]->ownerTag == TAG_ENEMY_MRFLUFFY || contacts[i]->ownerTag == TAG_ENEMY_MRICEBLOCK){
                    currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                }else{
                    currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                    currEnemyInstance->direction = 1;
                }
            }else{ // If right touching
                if(contacts[i]->ownerTag != TAG_HEROS_TUX){
                    if(currEnemyInstance->currentActionId != 4 && currEnemyInstance->currentActionId != 5){
                        changeEnemyAction(currEnemyInstance, 0, 0);
                    }
                }else{
                    if(currEnemyInstance->isStuck){
                        currEnemyInstance->isStuck = 0;
                        changeEnemyAction(currEnemyInstance, 4, 0);
                        currEnemyInstance->coll[currEnemyInstance->currentActionId]->ownerTag = TAG_BULL;
                    }
                }
                if( (currEnemyInstance->currentActionId == 4 || currEnemyInstance->currentActionId == 5)
                && contacts[i]->ownerTag == TAG_ENEMY_MRBOMB || contacts[i]->ownerTag == TAG_ENEMY_MRFLUFFY || contacts[i]->ownerTag == TAG_ENEMY_MRICEBLOCK){
                    currEnemyInstance->posX = contacts[i]->posX + contacts[i]->width;
                }else{
                    currEnemyInstance->posX = contacts[i]->posX - currEnemyInstance->coll[currEnemyInstance->currentActionId]->width;
                    currEnemyInstance->direction = -1;
                }

            }
        }
    }



    // Update Collider position
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posX = currEnemyInstance->posX;
    currEnemyInstance->coll[currEnemyInstance->currentActionId]->posY = currEnemyInstance->posY;
}//------------------------------------------------------------------------------------------------------------------------


