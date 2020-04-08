#include <stdio.h>
#include <SDL.h>
#include "types.h"
#include "game.cpp"

int window_width = 1440;
int window_height = 900;

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);

    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;

    window = SDL_CreateWindow(
        "Cayo Gato",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        0);
    if (!window)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_GetWindowSurface(window);

    GameState *game_state = game_init_state();

    GameDisplay game_display = {};
    game_display.pixels = (u32 *)surface->pixels;
    game_display.width = window_width;
    game_display.height = window_height;

    GameInput game_input = {};
    bool running = true;
    while (running)
    {
        // Reset one-press-at-a-time inputs
        game_input.down  = false;
        game_input.up    = false;
        game_input.left  = false;
        game_input.right = false;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (!event.key.repeat)
                {
                    if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        game_input.down = true;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        game_input.up = true;
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT)
                    {
                        game_input.left = true;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT)
                    {
                        game_input.right = true;
                    }
                }
                break;
            case SDL_KEYUP:
                if (!event.key.repeat)
                {
                    // ... probably need this for something ...
                }
                break;
            case SDL_QUIT:
                running = 0;
                break;
            }
        }

        if (SDL_MUSTLOCK(surface))
        {
            SDL_LockSurface(surface);
        }

        game_update_and_render(game_state, game_input, game_display);

        if (SDL_MUSTLOCK(surface))
        {
            SDL_UnlockSurface(surface);
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
