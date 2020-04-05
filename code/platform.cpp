#include <stdio.h>
#include <SDL.h>

#include "platform.h"
// #include "game.h"

int window_width = 1440;
int window_height = 900;

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    bool running = true;

    u8 color_offset = 0;

    while (running)
    {
        /////////////// Collect input ///////////

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

        //////////////// Simulate ///////////////

        //////////////// Render /////////////////

        // Clear to something
        SDL_SetRenderDrawColor(
            renderer,
            0 + color_offset,
            64 + color_offset,
            128 + color_offset,
            255
        );
        SDL_RenderClear(renderer);

        // Blit
        SDL_RenderPresent(renderer);

        color_offset++;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    printf("Goodbye world\n");

    return 0;
}
