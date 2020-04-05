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
        0
    );
    if (!window) {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_GetWindowSurface(window);

    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if (!event.key.repeat)
                    {
                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            // add_vector_this_frame = 1;
                        }
                        else if (event.key.keysym.sym == SDLK_x)
                        {
                            // is_dragging_x_basis = 1;
                        }
                        else if (event.key.keysym.sym == SDLK_y)
                        {
                            // is_dragging_y_basis = 1;
                        }
                    }
                    break;
                case SDL_KEYUP:
                    if (!event.key.repeat)
                    {
                        if (event.key.keysym.sym == SDLK_x)
                        {
                            // is_dragging_x_basis = 0;
                        }
                        else if (event.key.keysym.sym == SDLK_y)
                        {
                            // is_dragging_y_basis = 0;
                        }
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
        // TODO: we're just using the default pixel color format, better to
        // use what's best for us in the game layer and explicitly convert from
        // that here.
        game_update_and_render((u32*)surface->pixels, window_width, window_height);
        if (SDL_MUSTLOCK(surface))
        {
            SDL_UnlockSurface(surface);
        }
        SDL_UpdateWindowSurface(window);

    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    printf("Goodbye world\n");

    return 0;
}
