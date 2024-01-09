#include <stdio.h>

#include "sdl.h"
#include "tools.h"
#include "map.h"
#include "creature.h"
#include "natsel.h"

// Map width
#define MAP_WIDTH 50
// Map height
#define MAP_HEIGHT 50
// creature size in pixel (square)
#define CREATURE_SIZE 15

// total creature we want
#define TOTAL 25
// best creatures we keep
#define BEST 8
// how much similar from the best
#define SIMILAR 24
// rest is random

// how much iteration life will last
#define LIFE_LONG 60

// how much hidden train
#define TRAIN_HIDE 50000
// how much shown train after hidden ones
#define TRAIN 6

// framerate (in milli-seconds)
#define FRAME_RATE 200

int main()
{
    // init of random functions
    tool_init(MAP_WIDTH, MAP_HEIGHT);

    // init of the map
    struct map *map = map_init(MAP_WIDTH, MAP_HEIGHT);

    // init of the natural selection system
    natsel_init(TOTAL, BEST, SIMILAR, LIFE_LONG);

    // sdl stuff
    if (sdl_init(MAP_WIDTH, MAP_HEIGHT, CREATURE_SIZE, FRAME_RATE) == 1)
        return 1;

    // hidden trains
    train_hide(TRAIN_HIDE, 1);

    // shown train
    train(TRAIN, 0);

    // free everything
    quit_sdl();
    map_destroy();

    return 0;
}
