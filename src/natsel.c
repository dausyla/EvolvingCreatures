#include "natsel.h"

void init_population(int n)
{
    for (int i = 0; i < n; i++)
        new_random_on_map(RED);
    for (int i = 0; i < n; i++)
        new_random_on_map(GREEN);
    for (int i = 0; i < n; i++)
        new_random_on_map(BLUE);
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

void life_loop_hide(int d, int k, int n)
{
    for (int i = 0; i < n; i++)
    {
        map_next();
        keep_bests(k);
        repopulate(n);
    }
}

void train(int pop_size, int days, int gen, int keep)
{
    init_population(pop_size);
    for (int i = 0; i < gen; i++)
        life_loop_hide(days, keep, pop_size);
    life_loop(days, frame_rate);
}
