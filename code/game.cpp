#include <stdlib.h>

#include "game.h"

#define PIXELS_PER_TILE 20;

GameColor tile_color = {0xFF, 0x88, 0};
GameColor cat_color = {0, 0, 0};
GameColor bg_color = {0, 0, 0};

GameState *game_init_state()
{
    GameState *state = (GameState *)malloc(sizeof(GameState));
    state->cat_pos.x = 0;
    state->cat_pos.y = 0;
    return state;
}

u32 game_render_color(GameColor color)
{
    u32 result = (color.r << 16) | (color.g << 8) | color.b;
    return result;
}

void game_update_and_render(
    GameState *state,
    GameInput input,
    GameDisplay display)
{
    // SECTION: Process input
    if (input.down)
    {
        state->cat_pos.y++;
    }
    if (input.up)
    {
        state->cat_pos.y--;
    }
    if (input.right)
    {
        state->cat_pos.x++;
    }
    if (input.left)
    {
        state->cat_pos.x--;
    }

    // SECTION: Simulate
    // ...

    // SECTION: Render
    for (
        int pixel_row = 0;
        pixel_row < display.height;
        pixel_row++)
    {
        int tile_y = pixel_row / PIXELS_PER_TILE;

        for (
            int pixel_column = 0;
            pixel_column < display.width;
            pixel_column++)
        {
            u32 *pixel = display.pixels + (pixel_row * display.width) + pixel_column;

            int tile_x = pixel_column / PIXELS_PER_TILE;

            int column_pixel_in_tile = pixel_column % PIXELS_PER_TILE;
            int row_pixel_in_tile = pixel_row % PIXELS_PER_TILE;

            bool tile_pixel = (column_pixel_in_tile > 0) &&
                              (row_pixel_in_tile > 0);

            bool cat_tile = (tile_y == state->cat_pos.y) &&
                            (tile_x == state->cat_pos.x);

            bool cat_pixel = (8 <= column_pixel_in_tile) &&
                             (column_pixel_in_tile < 12) &&
                             (8 <= row_pixel_in_tile) &&
                             (row_pixel_in_tile < 12);

            if (cat_tile && cat_pixel)
            {
                *pixel = game_render_color(cat_color);
            }
            else if (tile_pixel)
            {
                *pixel = game_render_color(tile_color);
            }
            else
            {
                *pixel = game_render_color(bg_color);
            }
        }
    }
}
