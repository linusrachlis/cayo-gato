#include <stdlib.h>

#include "game.h"

#define TILE_SIZE 20;

GameColor tile_color = {0xAA, 0x66, 0x33};
GameColor cat_color = {0, 0, 0};
GameColor bg_color = {0, 0, 0};

GameState *game_init_state()
{
    GameState *state = (GameState *)malloc(sizeof(GameState));
    state->cat_pos.x = 4;
    state->cat_pos.y = 0;
    bool tile_map[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    memcpy(state->tile_map, tile_map, TILE_MAP_HEIGHT * TILE_MAP_WIDTH);
    return state;
}

u32 game_render_color(GameColor color)
{
    u32 result = (color.r << 16) | (color.g << 8) | color.b;
    return result;
}

void cat_move(GameState *state, int move_x, int move_y)
{
    int new_x = state->cat_pos.x + move_x;
    int new_y = state->cat_pos.y + move_y;

    if (
        new_x < 0 || new_y < 0 ||
        new_x >= TILE_MAP_WIDTH ||
        new_y >= TILE_MAP_HEIGHT)
    {
        return;
    }

    if (!state->tile_map[new_y][new_x])
    {
        return;
    }

    state->cat_pos.x = new_x;
    state->cat_pos.y = new_y;
}

void game_update_and_render(
    GameState *state,
    GameInput input,
    GameDisplay display)
{
    // SECTION: Process input
    if (input.down)
    {
        cat_move(state, 0, 1);
    }
    if (input.up)
    {
        cat_move(state, 0, -1);
    }
    if (input.right)
    {
        cat_move(state, 1, 0);
    }
    if (input.left)
    {
        cat_move(state, -1, 0);
    }

    // SECTION: Simulate
    // ...

    // SECTION: Render

    u32 rendered_cat_color = game_render_color(cat_color);
    u32 rendered_tile_color = game_render_color(tile_color);
    u32 rendered_bg_color = game_render_color(bg_color);

    for (
        int pixel_row = 0;
        pixel_row < display.height;
        pixel_row++)
    {
        int tile_y = pixel_row / TILE_SIZE;

        for (
            int pixel_column = 0;
            pixel_column < display.width;
            pixel_column++)
        {
            u32 *pixel = display.pixels + (pixel_row * display.width) + pixel_column;

            int tile_x = pixel_column / TILE_SIZE;

            bool is_tile = (tile_y < TILE_MAP_HEIGHT) && (tile_x < TILE_MAP_WIDTH) &&
                           state->tile_map[tile_y][tile_x];

            if (is_tile)
            {
                bool cat_tile = (tile_y == state->cat_pos.y) &&
                                (tile_x == state->cat_pos.x);

                int column_pixel_in_tile = pixel_column % TILE_SIZE;
                int row_pixel_in_tile = pixel_row % TILE_SIZE;

                if (cat_tile)
                {

                    bool cat_pixel = (8 <= column_pixel_in_tile) &&
                                     (column_pixel_in_tile < 12) &&
                                     (8 <= row_pixel_in_tile) &&
                                     (row_pixel_in_tile < 12);
                    if (cat_pixel)
                    {
                        *pixel = rendered_cat_color;
                        continue;
                    }
                }

                bool tile_pixel = (column_pixel_in_tile > 0) &&
                                  (row_pixel_in_tile > 0);
                if (tile_pixel)
                {
                    *pixel = rendered_tile_color;
                    continue;
                }
            }

            *pixel = rendered_bg_color;
        }
    }
}
