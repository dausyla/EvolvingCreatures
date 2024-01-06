#ifndef MAP_H
#define MAP_H

#include "creature.h"
#include "tools.h"
#include "sdl.h"



struct map
{
    int red_alive;
    int green_alive;
    int blue_alive;
    struct creature *head;
    struct creature ***map;
};


struct map *map_init(int w, int h);

void map_show();

void map_hide();

struct creature *map_get_head();

void map_next();

void map_fflush();

void new_random_on_map(int color);

void new_similar_on_map(struct creature *best);

void new_best_on_map(struct creature *best);

void creature_move(struct creature *creature);

void creature_kill(struct creature *creature);

void creature_reproduce(struct creature *creature);

struct creature **get_creatures_seen(struct creature *creature, struct creature *seen[]);

void creature_destroy_rec(struct creature *c);

void map_clear();

void map_destroy();

#endif /* ! MAP_H */
