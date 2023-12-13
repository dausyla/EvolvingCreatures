#include "sdl.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Rect rect;

static int window_w;
static int window_h;

int sdl_init()
{
    window_w = CREATURE_SIZE * MAP_WIDTH;
    window_h = CREATURE_SIZE * MAP_HEIGHT;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("evol_crea", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, window_w, window_h,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n",
               SDL_GetError());
        return 1;
    }

    rect.w = CREATURE_SIZE;
    rect.h = CREATURE_SIZE;

    return 0;
}

void update_new_creature(struct creature *crea)
{
    if (crea->color == RED)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (crea->color == GREEN)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    if (crea->color == BLUE)
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);

    rect.x = crea->x * CREATURE_SIZE;
    rect.y = crea->y * CREATURE_SIZE;
    SDL_RenderFillRect(renderer, &rect);
}

void update_less_creature(struct creature *crea)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    rect.x = crea->x * CREATURE_SIZE;
    rect.y = crea->y * CREATURE_SIZE;
    SDL_RenderFillRect(renderer, &rect);
}

void update_screen()
{
    SDL_RenderPresent(renderer);
}

void quit_sdl()
{
    // Destroy the window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();
}
