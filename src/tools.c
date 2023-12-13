#include "tools.h"

void tool_init()
{
    srand(2);
}

void get_direction_int(double output, struct creature *crea)
{
    int delta;
    if (output < 0.2)
        delta = 3; // go left
    else if (output < 0.5)
        delta = 0; // go straight
    else if (output < 0.8)
        delta = 1; // go right
    else
        delta = 2; // U turn
    crea->last_delta = crea->delta;
    crea->delta = delta;
    crea->direction = (crea->direction + delta) % 4;
}

void get_action_int(double act, struct creature *crea)
{
    int res;
    if (act > 0.8)
        res = KILL;
    else if (act > 0.2)
        res = REPRODUCE;
    else
        res = MOVE;
    crea->last_action = crea->action;
    crea->action = res;
}

int random_direction()
{
    return rand() % 4;
}

int random_color()
{
    return rand() % 3;
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

int random_weight()
{
    return rand() % 200 - 100;
}

int randomize_weight(int weight, int i)
{
    return weight + random_weight() / sqrt(i);
}

int rand_x()
{
    return rand() % MAP_WIDTH;
}
int rand_y()
{
    return rand() % MAP_HEIGHT;
}
