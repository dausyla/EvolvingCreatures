#ifndef NATSEL_H
#define NATSEL_H

#include <stdio.h>

#include "map.h"
#include "tools.h"
#include "creature.h"

void natsel_init(int tot, int b, int s, int ll);

void init_population();

void life_loop();

void life_loop_hide();

void repopulate();

void keep_bests();

void train(int gen, int init);

void train_hide(int gen, int init);

#endif /* ! NATSEL_H */
