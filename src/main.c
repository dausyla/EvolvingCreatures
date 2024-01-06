#include <stdio.h>

#include "sdl.h"
#include "tools.h"
#include "map.h"
#include "creature.h"
#include "natsel.h"

#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define CREATURE_SIZE 15

#define TOTAL 25
#define BEST 11
#define SIMILAR 11
#define LIFE_LONG 100

#define TRAIN_HIDE 10000
#define TRAIN 10

#define FRAME_RATE 50

int main()
{
    tool_init(MAP_WIDTH, MAP_HEIGHT);

    struct map *map = map_init(MAP_WIDTH, MAP_HEIGHT);

    natsel_init(TOTAL, BEST, SIMILAR, LIFE_LONG);

    if (sdl_init(MAP_WIDTH, MAP_HEIGHT, CREATURE_SIZE, FRAME_RATE) == 1)
        return 1;

    train_hide(TRAIN_HIDE, 1);

    train(TRAIN, 0);

    quit_sdl();

    map_destroy();

    return 0;
}
