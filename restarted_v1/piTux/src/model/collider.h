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


Collider *initCollider(int id, int width, int height, int posX, int posY, int isEnabled, int ownerTag);
Collider *initNonRegisteredCollider(int width, int height, int posX, int posY, int isEnabled, int ownerTag);
void destroyCollider(Collider *currCollider);


#endif
