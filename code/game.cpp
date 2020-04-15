#include <math.h>
#include <stdlib.h>
#include <string.h>

#define TILE_SIZE 40
#define TILE_MAP_WIDTH 20
#define TILE_MAP_HEIGHT 20

#include "game.h"

GameColor tile_color = {0xAA, 0x66, 0x33};
GameColor cat_color = {0, 0, 0};
GameColor dead_cat_color = {0x55, 0x55, 0x55};
GameColor food_color = {0x55, 0, 0};
GameColor goal_color = {0, 0xFF, 0xFF};
GameColor bg_color = {0, 0, 0};

u32 game_render_color(GameColor color)
{
    u32 result = (color.r << 16) | (color.g << 8) | color.b;
    return result;
}

u32 rendered_cat_color = game_render_color(cat_color);
u32 rendered_dead_cat_color = game_render_color(dead_cat_color);
u32 rendered_food_color = game_render_color(food_color);
u32 rendered_goal_color = game_render_color(goal_color);
u32 rendered_tile_color = game_render_color(tile_color);
u32 rendered_bg_color = game_render_color(bg_color);

int cat_size = TILE_SIZE / 4;
int cat_pixel_in_tile_start = TILE_SIZE / 2 - cat_size / 2;
int cat_pixel_in_tile_end   = TILE_SIZE / 2 + cat_size / 2;

int food_size = TILE_SIZE / 5;
int food_pixel_in_tile_start = TILE_SIZE / 2 - food_size / 2;
int food_pixel_in_tile_end   = TILE_SIZE / 2 + food_size / 2;

Food make_food(int x, int y)
{
    Food result = {};
    result.pos = (TileCoord){x, y};
    result.exists = true;
    return result;
}

GameState *game_init_state()
{
    GameState *state = (GameState *)malloc(sizeof(GameState));

    state->time = 0;
    state->delta_time = 0;
    state->cat_pos = (TileCoord){4, 0};
    state->cat_life = 23;
    state->goal_pos = (TileCoord){17, 4};
    state->won = false;

    state->food[0] = make_food(3, 2);
    state->food[1] = make_food(17, 11);
    state->food[2] = make_food(17, 6);
    state->num_food = 3;

    bool tile_map[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 4
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 5
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 6
        {0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0}, // 7
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0}, // 8
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, // 9
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 10
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 11
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 12
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 13
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 14
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 15
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 16
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 17
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 18
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 19
    };
    memcpy(state->tile_map, tile_map, TILE_MAP_HEIGHT * TILE_MAP_WIDTH);

    return state;
}

void cat_move(GameState *state, int move_x, int move_y)
{
    if (state->won || state->cat_life <= 0) return;

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

    state->cat_life--;
    state->cat_pos.x = new_x;
    state->cat_pos.y = new_y;
}

u32 water_pixel(GameState *state, int x, int y, f64 time)
{
    static int move_range = 33;

    u32 bg_color = !state->won ? 0x00112299 : 0x0000FFFF;
    u32 wave_color = !state->won ? 0x006677EE : 0x00FFFFFF;

    if (state->won)
    {
        time *= 5;
    }

    f64 sin_time_y = sin(time + y);
    f64 sin_y = sin((f64)y);
    f64 sin_unit = (sin_time_y + sin_y) / 2.0;
    int offset_x = (int)(sin_unit * move_range) + x;
    bool wave = ((y % 33) == 0) &&
                (
                    (
                        (offset_x + y)
                        % 72
                    )
                < 27);
    if (wave)
    {
        return wave_color;
    }
    else
    {
        return bg_color;
    }
}

void game_update_and_render(
    GameState *state,
    GameInput input,
    GameDisplay display)
{
    // SECTION: Simulate

    // Move cat based on input
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

    // Check win condition
    if (state->goal_pos.x == state->cat_pos.x &&
        state->goal_pos.y == state->cat_pos.y)
    {
        state->won = true;
    }

    // Take food if you're on the same tile
    for (
        size_t food_index = 0;
        food_index < state->num_food;
        food_index++)
    {
        if (!state->food[food_index].exists)
        {
            continue;
        }

        TileCoord food_pos = state->food[food_index].pos;
        if (food_pos.x == state->cat_pos.x &&
            food_pos.y == state->cat_pos.y)
        {
            state->cat_life += 5;
            state->food[food_index].exists = false;
        }
    }

    // SECTION: Render
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

            bool is_tile = (tile_y < TILE_MAP_HEIGHT) &&
                           (tile_x < TILE_MAP_WIDTH) &&
                           state->tile_map[tile_y][tile_x];

            int column_pixel_in_tile;
            int row_pixel_in_tile;
            bool cat_tile, tile_pixel;

            // Draw life bar
            u32 life_bar_color = 0x0000FF00;
            int life_bar_left = 20;
            int life_bar_right = life_bar_left + state->cat_life * 20;
            int life_bar_top = display.height - 40;
            int life_bar_bottom = display.height - 20;
            int life_bar_tile_size = 20;
            if (
                (pixel_row >= life_bar_top) &&
                (pixel_row < life_bar_bottom) &&
                (pixel_column >= life_bar_left) &&
                (pixel_column < life_bar_right) &&
                (((pixel_column - life_bar_left) % life_bar_tile_size) != (life_bar_tile_size - 1)))
            {
                *pixel = life_bar_color;
                goto next_pixel;
            }

            if (!is_tile)
            {
                *pixel = water_pixel(state, pixel_column, pixel_row, state->time);
                goto next_pixel;
            }

            column_pixel_in_tile = pixel_column % TILE_SIZE;
            row_pixel_in_tile = pixel_row % TILE_SIZE;

            cat_tile = (tile_y == state->cat_pos.y) &&
                       (tile_x == state->cat_pos.x);
            if (cat_tile)
            {
                bool cat_pixel = (cat_pixel_in_tile_start <= column_pixel_in_tile) &&
                                 (column_pixel_in_tile < cat_pixel_in_tile_end) &&
                                 (cat_pixel_in_tile_start <= row_pixel_in_tile) &&
                                 (row_pixel_in_tile < cat_pixel_in_tile_end);
                if (cat_pixel)
                {
                    *pixel = state->cat_life > 0 ? rendered_cat_color : rendered_dead_cat_color;
                    goto next_pixel;
                }
            }

            // Draw goal
            if (state->goal_pos.x == tile_x && state->goal_pos.y == tile_y)
            {
                // Reuse food dimensions
                bool goal_pixel = (food_pixel_in_tile_start <= column_pixel_in_tile) &&
                                    (column_pixel_in_tile < food_pixel_in_tile_end) &&
                                    (food_pixel_in_tile_start <= row_pixel_in_tile) &&
                                    (row_pixel_in_tile < food_pixel_in_tile_end);
                if (goal_pixel)
                {
                    *pixel = rendered_goal_color;
                    goto next_pixel;
                }
            }

            // Draw food
            for (
                size_t food_index = 0;
                food_index < state->num_food;
                food_index++)
            {
                if (!state->food[food_index].exists)
                {
                    continue;
                }

                TileCoord food_pos = state->food[food_index].pos;
                if (food_pos.x == tile_x && food_pos.y == tile_y)
                {
                    bool food_pixel = (food_pixel_in_tile_start <= column_pixel_in_tile) &&
                                      (column_pixel_in_tile < food_pixel_in_tile_end) &&
                                      (food_pixel_in_tile_start <= row_pixel_in_tile) &&
                                      (row_pixel_in_tile < food_pixel_in_tile_end);
                    if (food_pixel)
                    {
                        *pixel = rendered_food_color;
                        goto next_pixel;
                    }
                }
            }

            tile_pixel = (column_pixel_in_tile > 0) &&
                         (row_pixel_in_tile > 0);
            if (tile_pixel)
            {
                *pixel = rendered_tile_color;
                goto next_pixel;
            }
            else
            {
                *pixel = water_pixel(state, pixel_column, pixel_row, state->time);
            }

        next_pixel:
            continue;
        }
    }
}
