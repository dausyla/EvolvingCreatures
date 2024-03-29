#include "map.h"

static int x;
static int y;
static int dx;
static int dy;

static struct map map;

static int map_width;
static int map_height;

static int hide;

struct map *map_init(int w, int h)
{
    map_width = w;
    map_height = h;
    hide = 1;
    map.red_alive = 0;
    map.green_alive = 0;
    map.blue_alive = 0;
    map.head = NULL;
    map.map = malloc(sizeof(struct creator **) * map_width);
    for (int i = 0; i < w; i++)
    {
        map.map[i] = malloc(sizeof(struct creature *) * map_height);
        for (int j = 0; j < h; j++)
            map.map[i][j] = NULL;
    }
    return &map;
}

void map_show()
{
    hide = 0;
}

void map_hide()
{
    hide = 1;
}

struct creature *map_get_head()
{
    return map.head;
}

void map_next()
{
    struct creature *p = map.head;
    while (p)
    {
        creature_next(p);
        p = p->next;
    }
}

void map_fflush(int nb)
{
    sdl_clear();
    struct creature *p = map.head;
    while (p)
    {
        update_new_creature(p);
        p->score = 0;
        p = p->next;
    }
    map.green_alive = nb;
    map.red_alive = nb;
    map.blue_alive = nb;
}

static void push_front(struct creature *new)
{
    new->next = map.head;
    if (map.head)
        map.head->prev = new;
    new->prev = NULL;
    map.head = new;
    if (new->color == RED)
        map.red_alive += 1;
    if (new->color == GREEN)
        map.green_alive += 1;
    if (new->color == BLUE)
        map.blue_alive += 1;
}

static struct creature *map_get(int x, int y)
{
    while (x < 0)
        x += map_width;
    while (x >= map_width)
        x -= map_width;
    while (y < 0)
        y += map_height;
    while (y >= map_height)
        y -= map_height;
    return map.map[x][y];
}

static void map_set(int x, int y, struct creature *crea)
{
    while (x < 0)
        x += map_width;
    while (x >= map_width)
        x -= map_width;
    while (y < 0)
        y += map_height;
    while (y >= map_height)
        y -= map_height;
    map.map[x][y] = crea;
}

static int modw(int x)
{
    while (x < 0)
        x += map_width;
    while (x >= map_width)
        x -= map_width;
    return x;
}

static int modh(int x)
{
    while (x < 0)
        x += map_height;
    while (x >= map_height)
        x -= map_height;
    return x;
}

void new_random_on_map(int color)
{
    x = rand_x();
    y = rand_y();
    while (map.map[x][y])
    {
        x = rand_x();
        y = rand_y();
    }

    map.map[x][y] = new_random(x, y, color);
    push_front(map.map[x][y]);
    if (!hide)
        update_new_creature(map.map[x][y]);
}

void new_similar_on_map(struct creature *best)
{
    x = rand_x();
    y = rand_y();
    while (map.map[x][y])
    {
        x = rand_x();
        y = rand_y();
    }

    map.map[x][y] = new_similar(x, y, best);
    push_front(map.map[x][y]);
    if (!hide)
        update_new_creature(map.map[x][y]);
}

void new_best_on_map(struct creature *best)
{
    x = rand_x();
    y = rand_y();
    while (map.map[x][y])
    {
        x = rand_x();
        y = rand_y();
    }

    map.map[x][y] = new_best(x, y, best);
    push_front(map.map[x][y]);
    if (!hide)
        update_new_creature(map.map[x][y]);
}

static void get_dx_dy(struct creature *creature)
{
    if (creature->direction == TOP)
    {
        dx = 0;
        dy = -1;
    }
    else if (creature->direction == BOTTOM)
    {
        dx = 0;
        dy = 1;
    }
    else if (creature->direction == RIGHT)
    {
        dx = 1;
        dy = 0;
    }
    else
    {
        dx = -1;
        dy = 0;
    }
}

void creature_move(struct creature *creature)
{
    if (!hide)
        update_less_creature(creature);

    x = creature->x;
    y = creature->y;

    get_dx_dy(creature);

    if (!map_get(x + dx, y + dy))
    {
        map_set(x + dx, y + dy, creature);
        map.map[x][y] = NULL;
        creature->x = modw(x + dx);
        creature->y = modh(y + dy);
    }
    else
        creature->score -= 5;
    if (!hide)
        update_new_creature(creature);
}

static void unlink_creature(struct creature *rm)
{
    if (rm->prev)
        rm->prev->next = rm->next;
    else
        map.head = rm->next;

    if (rm->next)
        rm->next->prev = rm->prev;

    if (rm->color == RED)
        map.red_alive-=1;
    if (rm->color == GREEN)
        map.green_alive-=1;
    if (rm->color == BLUE)
        map.blue_alive-=1;
};


void creature_kill(struct creature *creature)
{
    x = creature->x;
    y = creature->y;

    get_dx_dy(creature);

    struct creature *killed = map_get(x + dx, y + dy);

    if (killed)
    {
        if (killed->color != creature->color)
            creature->score += 20;
        if (!hide)
            update_less_creature(killed);
        unlink_creature(killed);
        creature_death(killed);
        map_set(x + dx, y + dy, NULL);
    }
}

void creature_reproduce(struct creature *creature)
{
    x = creature->x;
    y = creature->y;

    get_dx_dy(creature);

    struct creature *other = map_get(x + dx, y + dy);

    if (other && other->color == creature->color)
    {
            int new_x = -1;
            int new_y = -1;
            int found = 0;
            for (int i = -1; i <= 1 && !found; i++)
                for (int j = -1; j <= 1 && !found; j++)
                    if (!map_get(x + i, y + j))
                    {
                        new_x = modw(x + i);
                        new_y = modh(y + j);
                    }
            if (new_x != -1 || new_y != -1)
            {
                map.map[new_x][new_y] = new_similar(new_x, new_y, creature);
                push_front(map.map[new_x][new_y]);
                if (!hide)
                    update_new_creature(map.map[new_x][new_y]);
                creature->score += 20;
            }
    }
}

struct creature **get_creatures_seen(struct creature *creature, struct creature *seen[])
{
    x = creature->x;
    y = creature->y;
    int index;
    if (creature->direction == TOP)
    {
        seen[0] = map_get(x, y - 1);
        seen[1] = map_get(x - 1, y - 1);
        seen[2] = map_get(x - 1, y);
        seen[3] = map_get(x - 1, y + 1);
        seen[4] = map_get(x, y + 1);
        seen[5] = map_get(x + 1, y + 1);
        seen[6] = map_get(x + 1, y);
        seen[7] = map_get(x + 1, y - 1);

        index = 8;
        for (int i = 2; i <= 5; i++)
            for (int j = -i; j <= i; j++)
                seen[index++] = map_get(x + j, y - i);
    }
    else if (creature->direction == BOTTOM)
    {
        seen[0] = map_get(x, y + 1);
        seen[1] = map_get(x + 1, y + 1);
        seen[2] = map_get(x + 1, y);
        seen[3] = map_get(x + 1, y - 1);
        seen[4] = map_get(x, y - 1);
        seen[5] = map_get(x - 1, y - 1);
        seen[6] = map_get(x - 1, y);
        seen[7] = map_get(x - 1, y + 1);

        index = 8;
        for (int i = 2; i <= 5; i++)
            for (int j = -i; j <= i; j++)
                seen[index++] = map_get(x - j, y + i);
    }
    else if (creature->direction == RIGHT)
    {
        seen[0] = map_get(x + 1, y);
        seen[1] = map_get(x + 1, y - 1);
        seen[2] = map_get(x, y - 1);
        seen[3] = map_get(x - 1, y - 1);
        seen[4] = map_get(x - 1, y);
        seen[5] = map_get(x - 1, y + 1);
        seen[6] = map_get(x, y + 1);
        seen[7] = map_get(x + 1, y + 1);

        index = 8;
        for (int i = 2; i <= 5; i++)
            for (int j = -i; j <= i; j++)
                seen[index++] = map_get(x + i, y + j);
    }
    else
    {
        seen[0] = map_get(x - 1, y);
        seen[1] = map_get(x - 1, y + 1);
        seen[2] = map_get(x, y + 1);
        seen[3] = map_get(x + 1, y + 1);
        seen[4] = map_get(x + 1, y);
        seen[5] = map_get(x + 1, y - 1);
        seen[6] = map_get(x, y - 1);
        seen[7] = map_get(x - 1, y - 1);

        index = 8;
        for (int i = 2; i <= 5; i++)
            for (int j = -i; j <= i; j++)
                seen[index++] = map_get(x - i, y - j);
    }
}


void creature_destroy_rec(struct creature *c)
{
    if (c)
    {
        creature_destroy_rec(c->next);
        map_set(c->x, c->y, NULL);
        creature_death(c);
    }
}

void map_clear()
{
    creature_destroy_rec(map.head);
    map.head = NULL;
}

void map_destroy()
{
    map_clear();
    for(int i = 0; i < map_width; i++)
        free(map.map[i]);
    free(map.map);
}
