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

// Game settings
#define GRAVITY_SPEED 400 // Speed in pixel/second

// The limit to kill the enemies out of it
#define DEAD_LIMIT_Y -200


enum ObjectTag{
    tag_tux = 20,

    tag_enemy_fluffy = 30,
    tag_enemy_bomb = 40,
    tag_enemy_iceblock = 50,


    tag_block_weak = 100,
    tag_block_mystery = 110,
    tag_block_strong = 130,


    tag_bonus_coin = 200,
    tag_bonus_egg = 210,
    tag_bonus_flower = 220,

    tag_explosion = 300
};
enum ObjectState{
    state_normal = 1,

    state_tux_small = 10,
    state_tux_big = 20,
    state_tux_fire = 30,

    state_block_hit_up = 40,
    state_block_hit_down = 41
};

#endif
