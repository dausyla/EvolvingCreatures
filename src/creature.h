#ifndef CREATURE_H
#define CREATURE_H

#define SEE_LENGTH 5

/* the input for brain is what the creature sees
 * each creature can see 5 cases in front of him and with 90 angle
 * it can sense also every case arround him
 * the seen input is then 34
 */
#define INPUT 40
#define HIDDEN 10
/* the first output of the brain is the action
 * the last one is the direction
 */
#define OUTPUT 2

#include <stdlib.h>

enum color
{
    RED,
    GREEN,
    BLUE
};

enum action
{
    KILL,
    REPRODUCE,
    MOVE
};

enum direction
{
    LEFT,
    TOP,
    RIGHT,
    BOTTOM
};

struct brain
{
    double **bias;
    double ***weight;
};

struct creature
{
    int id;
    int x;
    int y;
    int direction;
    struct brain *brain;
    int color;
    int generation;
    struct creature *next;
    struct creature *prev;
    int score;
    int delta;
    int last_delta;
    int action;
    int last_action;
    int is_best;
};

#include "map.h"
#include "tools.h"

void creature_next(struct creature *creature);

struct creature *new_random(int x, int y, int color);

struct creature *new_similar(int x, int y, struct creature *parent);

struct creature *new_best(int x, int y, struct creature *best);

void creature_death(struct creature *creature);

#endif /* ! CREATURE_H */
