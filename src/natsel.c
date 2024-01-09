#include "natsel.h"

static char *path;

static int nb_best;
static int nb_similar;
static int nb_random;
static int total;

static int life_long;

static struct creature *greens;
static struct creature *blues;
static struct creature *reds;
static int ng;
static int nb;
static int nr;

void natsel_init(int tot, int b, int s, int ll, char *p)
{
    path = p;
    total = tot;
    nb_best = b;
    nb_similar = s;
    nb_random = total - nb_best - nb_similar;
    life_long = ll;

    reds = calloc(sizeof(struct creature), nb_best);
    greens = calloc(sizeof(struct creature), nb_best);
    blues = calloc(sizeof(struct creature), nb_best);
    for (int i = 0; i < nb_best; i++)
    {
        reds[i].score = -10000000;
        greens[i].score = -10000000;
        blues[i].score = -10000000;
    }
}

void init_population()
{
    for (int i = 0; i < total; i++)
        new_random_on_map(RED);
    for (int i = 0; i < total; i++)
        new_random_on_map(GREEN);
    for (int i = 0; i < total; i++)
        new_random_on_map(BLUE);
}

void life_loop()
{
    map_fflush(total);
    update_screen();
    sdl_delay();
    for (int i = 0; i < life_long; i++)
    {
        map_next();
        update_screen();
        sdl_delay();
    }
    keep_bests();
    repopulate(total);
}

void life_loop_hide()
{
    for (int i = 0; i < life_long; i++)
        map_next();
    keep_bests();
    repopulate();
}

void repopulate()
{
    for (int i = 0; i < nb_best; i++)
    {
        new_best_on_map(reds + i);
        new_best_on_map(greens + i);
        new_best_on_map(blues + i);
    }

    for (int i = 0; i < nb_similar; i++)
    {
        new_similar_on_map(reds + i % nb_best);
        new_similar_on_map(greens + i % nb_best);
        new_similar_on_map(blues + i % nb_best);
    }

    for (int i = 0; i < nb_random; i++)
    {
        new_random_on_map(RED);
        new_random_on_map(GREEN);
        new_random_on_map(BLUE);
    }
}

static void save_new_best(struct creature *cr, struct creature *crs)
{
    cr->is_best = 1;
    struct creature temp;
    int i;
    for (i = 0; i < nb_best; i++)
        if (cr->id == crs[i].id)
        {
            crs[i].score = cr->score;
            while (i < nb_best - 1 && crs[i].score > crs[i + 1].score)
            {
                temp = crs[i];
                crs[i] = crs[i + 1];
                crs[i + 1] = temp;
            }
            return;
        }

    i = 0;
    while (i < nb_best - 1 && cr->score > crs[i + 1].score)
    {
        crs[i] = crs[i + 1];
        i++;
    }
    crs[i] = *cr;
}

void keep_bests()
{
    struct creature *cp = map_get_head();
    int i;
    int ig = 0;
    int ib = 0;
    int ir = 0;
    while (cp)
    {
        i = 0;
        if (cp->color == RED && cp->score > reds[0].score)
            save_new_best(cp, reds);
        if (cp->color == GREEN && cp->score > greens[0].score)
            save_new_best(cp, greens);
        if (cp->color == BLUE && cp->score > blues[0].score)
            save_new_best(cp, blues);
        cp = cp->next;
    }
    map_clear();
}

void train(int gen, int init)
{
    map_show();
    if (init)
        init_population();
    for (int i = 0; i < gen; i++)
    {
        life_loop();
        printf("gen number %d\n", i);
    }
}

void train_hide(int gen, int init)
{
    map_hide();
    if (init)
        init_population();
    for (int i = 0; i < gen; i++)
    {
        life_loop_hide();
        printf("gen number %d\n", i);
    }
}

static void write_brain(struct creature *crea, FILE *file)
{
}

static void read_brain(struct creature *crea, FILE *file)
{
}

void get_bests()
{
    FILE *file = fopen(path, "r");
    if (!file)
        errx(1, "No best brain file");
    struct creature *c;
    for (int i = 0; i < nb_best; i++)
    {
        read_brain(reds + i, file);
        read_brain(greens + i, file);
        read_brain(blues + i, file);
    }
}

void save_bests()
{
    FILE *file = fopen(path, "w");
    struct creature *c;
    for (int i = 0; i < nb_best; i++)
    {
        write_brain(reds + i, file);
        write_brain(greens + i, file);
        write_brain(blues + i, file);
    }
}
