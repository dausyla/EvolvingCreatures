#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <math.h>

#include "creature.h"
#include "map.h"

void tool_init();

double sigmoid(double x);
void get_direction_int(double output, struct creature *crea);
void get_action_int(double act, struct creature *crea);

int random_weight();
int randomize_weight(int weight, int i);
int random_sex();

int random_direction();
int random_color();

int rand_x();
int rand_y();

#endif /* ! TOOLS_H */
