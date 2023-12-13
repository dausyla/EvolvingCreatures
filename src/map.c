#include "map.h"

static int x;
static int y;
static int dx;
static int dy;

static struct map map;

struct map *map_init()
{
    map.alive = 0;
    map.head = NULL;
    for (int i = 0; i < MAP_WIDTH; i++)
        for (int j = 0; j < MAP_HEIGHT; j++)
            map.map[i][j] = NULL;
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

static void push_front(struct creature *new)
{
    new->next = map.head;
    if (map.head)
        map.head->prev = new;
    new->prev = NULL;
    map.head = new;
    map.alive += 1;
}

static struct creature *map_get(int x, int y)
{
    while (x < 0)
        x += MAP_WIDTH;
    while (x >= MAP_WIDTH)
        x -= MAP_WIDTH;
    while (y < 0)
        y += MAP_HEIGHT;
    while (y >= MAP_HEIGHT)
        y -= MAP_HEIGHT;
    return map.map[x][y];
}

static void map_set(int x, int y, struct creature *crea)
{
    while (x < 0)
        x += MAP_WIDTH;
    while (x >= MAP_WIDTH)
        x -= MAP_WIDTH;
    while (y < 0)
        y += MAP_HEIGHT;
    while (y >= MAP_HEIGHT)
        y -= MAP_HEIGHT;
    map.map[x][y] = crea;
}

static int modw(int x)
{
    while (x < 0)
        x += MAP_WIDTH;
    while (x >= MAP_WIDTH)
        x -= MAP_WIDTH;
    return x;
}

static int modh(int x)
{
    while (x < 0)
        x += MAP_HEIGHT;
    while (x >= MAP_HEIGHT)
        x -= MAP_HEIGHT;
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

    map.alive-=1;
};


void creature_kill(struct creature *creature)
{
    x = creature->x;
    y = creature->y;

    get_dx_dy(creature);

    struct creature *killed = map_get(x + dx, y + dy);

    if (killed)
    {
        if (killed->color == creature->color)
            creature->score -= 20;
        else
            creature->score += 50;
        update_less_creature(killed);
        unlink_creature(killed);
        creature_death(killed);
        map_set(x + dx, y + dy, NULL);
    }
    else
        creature->score -= 5;
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
                map.map[new_x][new_y] = new_child(creature);
                push_front(map.map[new_x][new_y]);
                update_new_creature(map.map[new_x][new_y]);
                creature->score += 100;
            }
    }
    else
        creature->score -= 10;
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
