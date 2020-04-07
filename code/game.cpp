#include "game.h"

GameState *game_init_state()
{
    GameState *state = (GameState *)malloc(sizeof(GameState));
    state->color_offset = 0;
    state->movement_offset = 0;
    return state;
}

void game_update_and_render(
    GameState *state,
    GameInput input,
    GameDisplay display)
{
    // SECTION: Process input
    if (input.down || input.right)
    {
        state->movement_offset++;
    }
    if (input.left || input.up)
    {
        state->movement_offset--;
    }

    // SECTION: Simulate
    int num_pixels = display.width * display.height;
    u8 base_red = state->color_offset;
    u8 base_green = 64 + state->color_offset;
    u8 base_blue = 128 + state->color_offset;

    // SECTION: Render
    for (int pixel_index = 0; pixel_index < num_pixels; pixel_index++)
    {
        u8 red = base_red + pixel_index + state->movement_offset;
        u8 green = base_green + pixel_index + state->movement_offset;
        u8 blue = base_blue + pixel_index + state->movement_offset;
        u32 pixel_value = (red << 16) | (green << 8) | blue;
        display.pixels[pixel_index] = pixel_value;
    }
    // state->color_offset++;
}
