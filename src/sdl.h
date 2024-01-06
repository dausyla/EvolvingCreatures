#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>

#include "creature.h"
#include "map.h"
#include "tools.h"

int sdl_init(int w, int h, int cz, int fr);

void sdl_delay();

void sdl_clear();

void update_new_creature(struct creature *crea);

void update_less_creature(struct creature *crea);

void update_screen();

void quit_sdl();

#endif /* ! SDL_H */
