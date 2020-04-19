#include <stdio.h>
#include <SDL.h>
#include "platform.h"
#include "game.cpp"

#define TARGET_SECONDS_PER_FRAME 0.016666667

int window_width = 800;
int window_height = 600;

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

    u64 perf_freq = SDL_GetPerformanceFrequency();
    u64 perf_count_start = SDL_GetPerformanceCounter();
    u64 perf_count_end_frame;

    // TODO Or should be TARGET_SECONDS_PER_FRAME?
    float frame_seconds = 0;

    while (running)
    {
        // Reset one-press-at-a-time inputs
        game_input.down = false;
        game_input.up = false;
        game_input.left = false;
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

        game_state->delta_time = frame_seconds;
        game_state->time += frame_seconds;

        game_update_and_render(game_state, game_input, game_display);

        if (SDL_MUSTLOCK(surface))
        {
            SDL_UnlockSurface(surface);
        }
        SDL_UpdateWindowSurface(window);

        u64 perf_count_end_render = SDL_GetPerformanceCounter();
        float render_seconds = (perf_count_end_render - perf_count_start) / (float)perf_freq;

        float seconds_to_wait = TARGET_SECONDS_PER_FRAME - render_seconds;
        if (seconds_to_wait > 0)
        {
            u32 ms_to_wait = (u32)(seconds_to_wait * 1000);
            SDL_Delay(ms_to_wait);
        }

        perf_count_end_frame = SDL_GetPerformanceCounter();
        frame_seconds = (perf_count_end_frame - perf_count_start) / (float)perf_freq;
        printf(
            "framerate: (potential %.2f, actual %.2f)\n",
            1.0 / render_seconds,
            1.0 / frame_seconds);

        perf_count_start = perf_count_end_frame;
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
