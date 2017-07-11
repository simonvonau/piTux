#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <stdlib.h>
#include <stdio.h>

#define MUSIC_BUFFER_LENGTH 1024

// The limit border outside the screen where an object can collide/interact with other objects
#define COLLIDER_MARGIN 200


// Max size of a configuration file line
#define LINE_SIZE_MAX 512
// Max size of a file path
#define FILE_PATH_SIZE_MAX 512

// Used in collision management
#define TAG_HEROS_TUX 1
#define TAG_BONUS_COIN 10
#define TAG_BONUS_EGG 20
#define TAG_BONUS_FIREFLOWER 30
#define TAG_BLOCK_WEAK 100
#define TAG_BLOCK_NORMAL 110
#define TAG_BLOCK_STRONG 120
#define TAG_ENEMY_MRFLUFFY 200
#define TAG_ENEMY_MRICEBLOCK 210
#define TAG_ENEMY_MRBOMB 220
#define TAG_EXPLOSION 500
#define TAG_BULL 600 // When the icecube run fast and destroy everything
#define TAG_DEAD_ENEMY 1000

// Game settings
#define GRAVITY_SPEED 400 // Speed in pixel/second

// The limit to kill the enemies out of it
#define DEAD_LIMIT_Y -200


#endif
