#ifndef SDL_H
#define SDL_H

#define CREATURE_SIZE 5

#include <SDL2/SDL.h>

#include "creature.h"
#include "map.h"
#include "tools.h"

int sdl_init();

void update_new_creature(struct creature *crea);

void update_less_creature(struct creature *crea);

void update_screen();

void quit_sdl();

#endif /* ! SDL_H */
