#include <stdio.h>

#include "sdl.h"
#include "tools.h"
#include "map.h"
#include "creature.h"

int main()
{
    tool_init();

    //if (sdl_init() == 1)
    //    return 1;

    struct map *map = map_init();

    add_random_creature(100);
    for (int i = 0; i < 10; i++)
        life_loop_hide(500);

    struct creature *p = map->head;
    int i = 0;
    while (p)
    {
        printf("score %d : %d\n", i, p->score);
        p = p->next;
        i++;
    }

    //SDL_Delay(2000);

    //quit_sdl();

    return 0;
}
