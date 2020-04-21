#include <stdlib.h>
#include <string.h>

#define TILE_SIZE 40
#define TILE_MAP_WIDTH 20
#define TILE_MAP_HEIGHT 20
#define MAX_ENTITY_COUNT 32

#include "game.h"
#include "entities.cpp"
#include "simulate.cpp"
#include "render.cpp"

u32 tile_color = 0x00AA6633;
u32 cat_color = 0;
u32 dead_cat_color = 0x00555555;
u32 food_color = 0x00550000;
u32 goal_color = 0x0000FFFF;
u32 bg_color = 0;

int cat_size = TILE_SIZE / 4;
int cat_pixel_in_tile_start = TILE_SIZE / 2 - cat_size / 2;
int cat_pixel_in_tile_end   = TILE_SIZE / 2 + cat_size / 2;

int food_size = TILE_SIZE / 5;
int food_pixel_in_tile_start = TILE_SIZE / 2 - food_size / 2;
int food_pixel_in_tile_end   = TILE_SIZE / 2 + food_size / 2;

GameState *game_init_state()
{
    GameState *state = new GameState();

    state->time = 0;
    state->delta_time = 0;
    state->cat = make_cat(4, 0, 23);
    state->goal = make_goal(17, 4);
    add_entity(state, state->cat);
    add_entity(state, state->goal);
    state->won = false;

    add_entity(state, make_food(3, 2));
    add_entity(state, make_food(17, 11));
    add_entity(state, make_food(17, 6));

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

void game_update_and_render(
    GameState *state,
    GameInput input,
    GameDisplay display)
{
    // SECTION: Simulate

    // Move cat based on input
    // TODO: can you move twice in the same frame?
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
            static u32 life_bar_color = 0x0000FF00;
            static int life_bar_left = 20;
            int life_bar_right = life_bar_left + state->cat->life * 20;
            int life_bar_top = display.height - 40;
            int life_bar_bottom = display.height - 20;
            static int life_bar_tile_size = 20;
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
                // Shortcut to draw water
                *pixel = water_pixel(state, pixel_column, pixel_row, state->time);
                goto next_pixel;
            }

            column_pixel_in_tile = pixel_column % TILE_SIZE;
            row_pixel_in_tile = pixel_row % TILE_SIZE;

            // Draw entities before tiles
            for (
                size_t entity_index = 0;
                entity_index < state->entity_count;
                entity_index++)
            {
                Entity *entity = state->entities[entity_index];
                switch (entity->type)
                {
                    case ENTITY_FOOD: {
                        Food *food = (Food *)entity;
                        if (!food->exists)
                        {
                            continue;
                        }
                                
                        if (food->pos.x == tile_x && food->pos.y == tile_y)
                        {
                            bool food_pixel = (food_pixel_in_tile_start <= column_pixel_in_tile) &&
                                            (column_pixel_in_tile < food_pixel_in_tile_end) &&
                                            (food_pixel_in_tile_start <= row_pixel_in_tile) &&
                                            (row_pixel_in_tile < food_pixel_in_tile_end);
                            if (food_pixel)
                            {
                                *pixel = food_color;
                                goto next_pixel;
                            }
                        }
                    } break;

                    case ENTITY_GOAL:
                    {
                        if (entity->pos.x == tile_x && entity->pos.y == tile_y)
                        {
                            // Reuse food dimensions
                            bool goal_pixel = (food_pixel_in_tile_start <= column_pixel_in_tile) &&
                                              (column_pixel_in_tile < food_pixel_in_tile_end) &&
                                              (food_pixel_in_tile_start <= row_pixel_in_tile) &&
                                              (row_pixel_in_tile < food_pixel_in_tile_end);
                            if (goal_pixel)
                            {
                                *pixel = goal_color;
                                goto next_pixel;
                            }
                        }
                    }
                    break;

                    case ENTITY_CAT:
                    {
                        Cat *cat = (Cat *)entity;
                        cat_tile = (tile_y == cat->pos.y) &&
                                   (tile_x == cat->pos.x);
                        if (cat_tile)
                        {
                            bool cat_pixel = (cat_pixel_in_tile_start <= column_pixel_in_tile) &&
                                             (column_pixel_in_tile < cat_pixel_in_tile_end) &&
                                             (cat_pixel_in_tile_start <= row_pixel_in_tile) &&
                                             (row_pixel_in_tile < cat_pixel_in_tile_end);
                            if (cat_pixel)
                            {
                                *pixel = cat->life > 0 ? cat_color : dead_cat_color;
                                goto next_pixel;
                            }
                        }
                    }
                    break;
                }
            }

            tile_pixel = (column_pixel_in_tile > 0) &&
                         (row_pixel_in_tile > 0);
            if (tile_pixel)
            {
                *pixel = tile_color;
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
