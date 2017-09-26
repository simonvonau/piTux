#ifndef __COLLIDER_H__
#define __COLLIDER_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy strncpy

#include "../utils/reporting.h"

typedef struct{
    int id;
    int width,height;
    int posX, posY;
    int lastPosX, lastPosY; // Last posX and posY=> required to process collisions
    int isEnabled;
    int ownerTag; // Global tag (cf global.h)
}Collider;


Collider *initCollider(int p_id, int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, int p_ownerTag);
Collider *initNonRegisteredCollider(int p_width, int p_height, int p_posX, int p_posY, int p_isEnabled, int p_ownerTag);
void destroyCollider(Collider *p_collider);


#endif
