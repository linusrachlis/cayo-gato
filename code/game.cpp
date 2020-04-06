#include "game.h"

GameState* game_init_state()
{
    GameState* state = (GameState*)malloc(sizeof(GameState));
    state->color_offset = 0;
    return state;
}

void game_update_and_render(
    GameState* state,
    GameInput input,
    GameDisplay display)
{
    int num_pixels = display.width * display.height;
    u8 red = state->color_offset;
    u8 green = 64 + state->color_offset;
    u8 blue = 128 + state->color_offset;

    for (int pixel_index = 0; pixel_index < num_pixels; pixel_index++)
    {
        u32 pixel_value = (red << 16) | (green << 8) | blue;
        display.pixels[pixel_index] = pixel_value;
    }
    state->color_offset++;
}
