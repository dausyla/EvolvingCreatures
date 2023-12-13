#include <stdio.h>

#include "sdl.h"
#include "tools.h"
#include "map.h"
#include "creature.h"

void add_random_creature(int n)
{
    for (int i = 0; i < n; i++)
        new_random_on_map(random_color());
}

void life_loop(int n, int d)
{
    update_screen();
    SDL_Delay(d);
    for (int i = 0; i < n; i++)
    {
        map_next();
        update_screen();
        SDL_Delay(d);
    }
}

void life_loop_hide(int n, int d)
{
    for (int i = 0; i < n; i++)
        map_next();
}

int main()
{
    tool_init();

    if (sdl_init() == 1)
        return 1;

    struct map *map = map_init();

    add_random_creature(100);
    life_loop(250, 50);

    struct creature *p = map->head;
    int i = 0;
    while (p)
    {
        printf("score %d : %d\n", i, p->score);
        p = p->next;
        i++;
    }

    SDL_Delay(2000);

    quit_sdl();

    return 0;
}
