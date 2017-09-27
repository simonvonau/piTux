#include "fireBulletInstance.h"


FireBulletInstance * initFireBulletInstance(int p_posX, int p_posY, int p_directionX, int p_directionY, Collider *p_coll, int p_lifeTimeLeft){
// Initialize a FireBulletInstance
    FireBulletInstance *res = malloc(sizeof(FireBulletInstance));

    if(res == NULL){
        reportErreur("Error malloc initFireBulletInstance()");
    }
    res->posX = p_posX;
    res->posY = p_posY;
    res->movementX = 0;
    res->movementY = 0;
    res->directionX = p_directionX;
    res->directionY = p_directionY;
    res->lastMinY = p_posY;
    res->coll = p_coll;
    res->currentSpriteId = 0;
    res->lifeTimeLeft = p_lifeTimeLeft;
    res->currentTime = 0;

    return res;
}//------------------------------------------------------------------------------------------------------------------------

void refreshFireBulletInstance(FireBulletInstance *p_fbi, FireBullet p_fireBullet, int p_loopTime){
// Refresh a fireBulletInstance
    int movementX, movementY;
    p_fbi->currentTime += p_loopTime;
    p_fbi->lifeTimeLeft -= p_loopTime;

    // Vertical & lateral motion
    p_fbi->movementX += p_fireBullet.speedX * p_fbi->directionX * p_loopTime / 1000.0;
    movementX = (int) p_fbi->movementX;
    p_fbi->movementY += p_fireBullet.speedY * p_fbi->directionY * p_loopTime / 1000.0;
    movementY = (int) p_fbi->movementY;

    if(movementX <= -1 || movementX >= 1){
        p_fbi->posX += movementX;
        p_fbi->movementX -= movementX;
    }
    if(movementY <= -1 || movementY >= 1){
        p_fbi->posY += movementY;
        p_fbi->movementY -= movementY;

        // Store the lowest position of the bullet (to determine its max high)
        if(p_fbi->posY < p_fbi->lastMinY){
            p_fbi->lastMinY = p_fbi->posY;
        }
    }

    // When the bullet is at its maw height it has to go down
    if(p_fbi->posY - p_fbi->lastMinY >= p_fireBullet.maxJumpHeight){
        p_fbi->directionY = -1;
    }

    // Refreshing the displayed sprite id
    if(p_fbi->currentTime >= p_fireBullet.spriteDuration){
        p_fbi->currentSpriteId += 1;
        p_fbi->currentTime = 0;
        p_fbi->currentSpriteId = p_fbi->currentSpriteId % p_fireBullet.spritesSize1;
    }

    p_fbi->coll->lastPosX = p_fbi->coll->posX;
    p_fbi->coll->lastPosY = p_fbi->coll->posY;
    p_fbi->coll->posX = p_fbi->posX;
    p_fbi->coll->posY = p_fbi->posY;

}//------------------------------------------------------------------------------------------------------------------------



void destroyFireBulletInstance(FireBulletInstance *p_fireBulletInstance){
// Free a fireBulletInstance
    destroyCollider(p_fireBulletInstance->coll);
    free(p_fireBulletInstance);
}//------------------------------------------------------------------------------------------------------------------------

