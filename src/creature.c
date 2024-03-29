#include "creature.h"

// input layer used for every neural network (explained in creature.h)
static double input[INPUT];
// hidden layer
static double hidden[HIDDEN];
// output layer (explained in creature.h)
static double output[OUTPUT];

// action decided by creatures
static int action;

// all creatures seen by one creature (used for the input layer)
static struct creature *seen[INPUT];

// id for the next creature to be initialized
static int id;

/* uses the brain of the creature to decide next action
 * (does the actual neural network matrix multiplication)
 * action is then set
 * direction is set in the creature struct */
static void brainize(struct creature *creature, struct creature **seen)
{
    double ***weight = creature->brain->weight;
    double **bias = creature->brain->bias;

    for (int i = 0; i < INPUT - 3; i++)
    {
        input[i] = 0.0;
        if (seen[i])
            input[i] += seen[i]->color + 1.0;
    }

    for (int j = 0; j < HIDDEN; j++)
    {
        hidden[j] = 0.0;
        for (int i = 0; i < INPUT; i++)
            hidden[j] += input[i] * weight[0][i][j];
        hidden[j] -= bias[0][j];
        hidden[j] = sigmoid(hidden[j]);
    }

    for (int k = 0; k < OUTPUT; k++)
    {
        output[k] = 0.0;
        for (int j = 0; j < HIDDEN; j++)
            output[k] += hidden[j] * weight[1][j][k];
        output[k] -= bias[1][k];
        output[k] = sigmoid(output[k]);
    }

    get_direction_int(output[1], creature);

    get_action_int(output[0], creature);
}

// update the score of the creature depending his choice this round
static void get_score(struct creature *c)
{
    if (c->action != c->last_action)
        c->score += 3;
    if (c->last_delta != c->delta)
        c->score += 2;
}

// get the whole next status of the creature
void creature_next(struct creature *creature)
{
    get_creatures_seen(creature, seen);
    brainize(creature, seen);
    get_score(creature);
    if (creature->action == MOVE)
        creature_move(creature);
    else if (creature->action == REPRODUCE)
        creature_reproduce(creature);
    else
        creature_kill(creature);
}

/* fill the brain of the new creature
 * from a parent creature depending its generation */
static void fill_brain_parent(struct brain *brain, struct brain *parent, int generation)
{
    brain->weight = malloc(sizeof(double **) * 2);
    brain->weight[0] = malloc(sizeof(double *) * INPUT);
    for (int i = 0; i < INPUT; i++)
    {
        brain->weight[0][i] = malloc(sizeof(double) * HIDDEN);
        for (int j = 0; j < HIDDEN; j++)
            brain->weight[0][i][j] = randomize_weight(parent->weight[0][i][j], generation);
    }
    brain->weight[1] = malloc(sizeof(double *) * HIDDEN);
    for (int i = 0; i < HIDDEN; i++)
    {
        brain->weight[1][i] = malloc(sizeof(double) * OUTPUT);
        for (int j = 0; j < OUTPUT; j++)
            brain->weight[1][i][j] = randomize_weight(parent->weight[1][i][j], generation);
    }

    brain->bias = malloc(sizeof(double *) * 2);
    brain->bias[0] = malloc(sizeof(double) * HIDDEN);
    brain->bias[1] = malloc(sizeof(double) * OUTPUT);

    for (int i = 0; i < HIDDEN; i++)
        brain->bias[0][i] = randomize_weight(parent->bias[0][i], generation);
    for (int i = 0; i < OUTPUT; i++)
        brain->bias[1][i] = random_weight(parent->bias[1][i], generation);
}

// fill the brain with fully random weight and bias
static void fill_brain_random(struct brain *brain)
{
    brain->weight = malloc(sizeof(double **) * 2);
    brain->weight[0] = malloc(sizeof(double *) * INPUT);
    brain->weight[1] = malloc(sizeof(double *) * HIDDEN);
    for (int i = 0; i < INPUT; i++)
    {
        brain->weight[0][i] = malloc(sizeof(double) * HIDDEN);
        for (int j = 0; j < HIDDEN; j++)
            brain->weight[0][i][j] = random_weight();
    }
    for (int i = 0; i < HIDDEN; i++)
    {
        brain->weight[1][i] = malloc(sizeof(double) * OUTPUT);
        for (int j = 0; j < OUTPUT; j++)
            brain->weight[1][i][j] = random_weight();
    }

    brain->bias = malloc(sizeof(double *) * 2);
    brain->bias[0] = malloc(sizeof(double) * HIDDEN);
    brain->bias[1] = malloc(sizeof(double) * OUTPUT);

    for (int i = 0; i < HIDDEN; i++)
        brain->bias[0][i] = random_weight();
    for (int i = 0; i < OUTPUT; i++)
        brain->bias[1][i] = random_weight();
}

// create a totally new random creature
struct creature *new_random(int x, int y, int color)
{
    struct creature *new = malloc(sizeof(struct creature));
    new->id = id++;
    new->x = x;
    new->y = y;
    new->is_best = 0;

    new->direction = random_direction();
    new->color = color;
    new->generation = 1;

    struct brain *brain = malloc(sizeof(struct brain));

    fill_brain_random(brain);

    new->brain = brain;
    new->score = 0;
    return new;
}

// create a new creature from a parent and a generation (during its life)
struct creature *new_best(int x, int y, struct creature *best)
{
    struct creature *new = malloc(sizeof(struct creature));
    new->id = best->id;
    new->x = x;
    new->y = y;
    new->is_best = 1;

    new->direction = best->direction;
    new->color = best->color;
    new->generation = best->generation + 1;

    new->brain = best->brain;
    new->score = 0;
    return new;
}

// create a similar creature from a parent (for a new life simulation)
struct creature *new_similar(int x, int y, struct creature *parent)
{
    struct creature *new = malloc(sizeof(struct creature));
    new->id = id++;
    new->x = x;
    new->y = y;
    new->is_best = 0;

    new->direction = parent->direction;
    new->color = parent->color;
    new->generation = parent->generation + 1;

    struct brain *brain = malloc(sizeof(struct brain));

    fill_brain_parent(brain, parent->brain, new->generation);

    new->brain = brain;
    new->score = 0;
    return new;
}

// free all the memory used by the creature and destroys it
void creature_death(struct creature *creature)
{
    if (creature)
    {
        if (!creature->is_best)
        {
            for (int i = 0; i < INPUT; i++)
                free(creature->brain->weight[0][i]);
            for (int i = 0; i < HIDDEN; i++)
                free(creature->brain->weight[1][i]);
            free(creature->brain->weight[0]);
            free(creature->brain->weight[1]);
            free(creature->brain->weight);

            free(creature->brain->bias[0]);
            free(creature->brain->bias[1]);
            free(creature->brain->bias);

            free(creature->brain);
        }
        free(creature);
    }
}
