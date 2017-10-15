#include "enemyBehaviour.h"

void updateEnemyBehaviourAfterCollisionDetection(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime){
// Chooses the right behaviour matching with p_enemyInstance
    switch(p_enemyInstance->idEnemy){
        case 0:
            updateMrFluffyBehaviour(p_enemyInstance, p_enemy, p_collMgr, p_loopTime);
            break;
        case 1:
            updateMrBombBehaviour(p_enemyInstance, p_enemy, p_collMgr, p_loopTime);
            break;
        case 2:
            updateMrIceBlockBehaviour(p_enemyInstance, p_enemy, p_collMgr, p_loopTime);
            break;
        default:
            reportErreur("Cannot update this enemy Id! (doesn't exists)");
            break;
    }
}//------------------------------------------------------------------------------------------------------------------------

void updateMrFluffyBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime){
// Update Mr Fluffy behaviour
    int i;
    Collider **contacts;
    int contactsSize;
    int enemyFallY; // The deepness the enemy go down on the ground (due to gravity)
    int newX = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX;
    int newY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY;
    int decalX = 0;
    int decalY = 0;

    // Collision detection
    getColliderTouching(p_collMgr, p_enemyInstance->coll[p_enemyInstance->currentActionId]->id, &contacts, &contactsSize);

    enemyFallY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY - p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY;
    for (i = 0; i < contactsSize; i++){
        if(contacts[i]->ownerTag != tag_bonus_coin && contacts[i]->ownerTag != tag_bonus_egg && contacts[i]->ownerTag != tag_bonus_flower){
            // Dead after it touched an explosion or a block hit by Tux
            if(contacts[i]->ownerTag == tag_explosion || contacts[i]->ownerState == state_block_hit_up){
                if(p_enemyInstance->direction == -1){
                    changeEnemyAction(p_enemyInstance, 4, 0, &decalX, &decalY);
                }else{
                    changeEnemyAction(p_enemyInstance, 5, 0, &decalX, &decalY);
                }
                newX += decalX;
                newY += decalY;
                decalX = 0;
                decalY = 0;
                p_enemyInstance->isDead = 1;
                p_enemyInstance->lifeTime = 1000;
                p_enemyInstance->coll[p_enemyInstance->currentActionId]->isEnabled = 0;
                return;
            }

            // If the colliding object is under
            if( p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
                newY = contacts[i]->posY + contacts[i]->height;
            // If the colliding object is above
            }else if(p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY + p_enemyInstance->coll[p_enemyInstance->currentActionId]->height <= contacts[i]->lastPosY + enemyFallY){
                if(!p_enemyInstance->isDead && contacts[i]->ownerTag == tag_tux){
                    playSoundByID(currMusicManager, 12);
                    if(p_enemyInstance->direction == -1){
                        changeEnemyAction(p_enemyInstance, 2, 0, &decalX, &decalY);
                    }else{
                        changeEnemyAction(p_enemyInstance, 3, 0, &decalX, &decalY);
                    }
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                    p_enemyInstance->isDead = 1;
                    p_enemyInstance->lifeTime = 500;
                    p_enemyInstance->coll[p_enemyInstance->currentActionId]->isEnabled = 0;
                    p_enemyInstance->isExposedToGravity = 0;
                }
            }else if(!p_enemyInstance->isDead){ // Lateral collision
                // If left touching
                if (p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX > contacts[i]->posX){
                    changeEnemyAction(p_enemyInstance, 1, 0, &decalX, &decalY);
                    newX = contacts[i]->posX + contacts[i]->width;
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                    p_enemyInstance->direction = 1;
                }else{ // If right touching
                    changeEnemyAction(p_enemyInstance, 0, 0, &decalX, &decalY);
                    newX = contacts[i]->posX - p_enemyInstance->coll[p_enemyInstance->currentActionId]->width;
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                    p_enemyInstance->direction = -1;
                }
            }
        }
    }
    // Update Collider position
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX = newX;
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY = newY;

    updEneSpriteLocFromCollLoc(p_enemyInstance, &p_enemy);

}//------------------------------------------------------------------------------------------------------------------------

void updateMrBombBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime){
// Update Mr Bomb behaviour
    int i;
    Collider **contacts;
    int contactsSize;
    int enemyFallY; // The deepness the enemy go down on the ground (due to gravity)
    int newX = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX;
    int newY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY;
    int decalX = 0;
    int decalY = 0;

    // Collision detection
    getColliderTouching(p_collMgr, p_enemyInstance->coll[p_enemyInstance->currentActionId]->id, &contacts, &contactsSize);

    for (i = 0; i < contactsSize; i++){
        enemyFallY = p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY - p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY;
        if(contacts[i]->ownerTag != tag_bonus_coin && contacts[i]->ownerTag != tag_bonus_egg && contacts[i]->ownerTag != tag_bonus_flower){
            // Mr Bomb explodes when he touches an explosion
            if((contacts[i]->ownerTag == tag_explosion || contacts[i]->ownerState == state_block_hit_up) && p_enemyInstance->currentActionId != 4){
                playSoundByID(currMusicManager, 2);
                changeEnemyAction(p_enemyInstance, 4, 0, &decalX, &decalY);
                p_enemyInstance->coll[p_enemyInstance->currentActionId]->ownerTag = tag_explosion;
                p_enemyInstance->isDead = 1;
                p_enemyInstance->isExposedToGravity = 0;
                // Get the explosion duration (spritesQty * spriteDuration)
                p_enemyInstance->lifeTime = p_enemy.spriteDuration[p_enemyInstance->currentActionId] * p_enemy.spritesSize2[p_enemyInstance->currentActionId];
                newX += decalX;
                newY += decalY;
                decalX = 0;
                decalY = 0;
            }

            // If the colliding object is under
            if( p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY >= contacts[i]->posY + contacts[i]->height){
                newY = contacts[i]->posY + contacts[i]->height;
            // If the colliding object is above
            }else if(p_enemyInstance->coll[p_enemyInstance->currentActionId]->lastPosY + p_enemyInstance->coll[p_enemyInstance->currentActionId]->height <= contacts[i]->lastPosY + enemyFallY){
                if(!p_enemyInstance->isDead && contacts[i]->ownerTag == tag_tux){
                    if(p_enemyInstance->direction == -1){
                        changeEnemyAction(p_enemyInstance, 2, 0, &decalX, &decalY);
                    }else{
                        changeEnemyAction(p_enemyInstance, 3, 0, &decalX, &decalY);
                    }
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                    p_enemyInstance->lifeTime = p_enemy.spriteDuration[p_enemyInstance->currentActionId] * p_enemy.spritesSize2[p_enemyInstance->currentActionId] + 200;
                    p_enemyInstance->isDead = 1;
                }
            }else if(!p_enemyInstance->isDead){ // Lateral collision
                // If left touching
                if (p_enemyInstance->posX > contacts[i]->posX){
                    changeEnemyAction(p_enemyInstance, 1, 0, &decalX, &decalY);
                    newX = contacts[i]->posX + contacts[i]->width;
                    p_enemyInstance->direction = 1;
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                }else{ // If right touching
                    changeEnemyAction(p_enemyInstance, 0, 0, &decalX, &decalY);
                    newX = contacts[i]->posX - p_enemyInstance->coll[p_enemyInstance->currentActionId]->width;
                    p_enemyInstance->direction = -1;
                    newX += decalX;
                    newY += decalY;
                    decalX = 0;
                    decalY = 0;
                }
            }
        }
    }

    // Create the explosion
    if((p_enemyInstance->currentActionId == 2 || p_enemyInstance->currentActionId == 3)
       && p_enemyInstance->currentSpriteId == p_enemy.spritesSize2[p_enemyInstance->currentActionId] - 1){
        playSoundByID(currMusicManager, 2);
        changeEnemyAction(p_enemyInstance, 4, 0, &decalX, &decalY);
        //newX += decalX;
        decalX = 0;
        decalY = 0;
        p_enemyInstance->coll[p_enemyInstance->currentActionId]->ownerTag = tag_explosion;
        p_enemyInstance->isDead = 1;
        p_enemyInstance->isExposedToGravity = 0;
        // Get the explosion duration (spritesQty * spriteDuration)
        p_enemyInstance->lifeTime = p_enemy.spriteDuration[p_enemyInstance->currentActionId] * p_enemy.spritesSize2[p_enemyInstance->currentActionId];
    }

    // Update Collider position
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posX = newX;
    p_enemyInstance->coll[p_enemyInstance->currentActionId]->posY = newY;

    updEneSpriteLocFromCollLoc(p_enemyInstance, &p_enemy);
}//------------------------------------------------------------------------------------------------------------------------

void updateMrIceBlockBehaviour(EnemyInstance *p_enemyInstance, Enemy p_enemy, ColliderManager *p_collMgr, int p_loopTime){
// Update Mr IceBlock behaviour
    // Disabled
}//------------------------------------------------------------------------------------------------------------------------


