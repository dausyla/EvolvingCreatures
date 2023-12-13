#ifndef MAP_H
#define MAP_H

#define MAP_HEIGHT 100
#define MAP_WIDTH 100

#include "creature.h"
#include "tools.h"
#include "sdl.h"

struct map
{
    int alive;
    struct creature *head;
    struct creature *map[MAP_WIDTH][MAP_HEIGHT];
};


struct map *map_init();

void map_next();

void new_random_on_map(int color);

void creature_move(struct creature *creature);

void creature_kill(struct creature *creature);

void creature_reproduce(struct creature *creature);

struct creature **get_creatures_seen(struct creature *creature, struct creature *seen[]);


#endif /* ! MAP_H */
