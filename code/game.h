#include "types.h"

#define TILE_MAP_WIDTH 10
#define TILE_MAP_HEIGHT 10

struct GameDisplay
{
    u32 *pixels;
    int width, height;
};

struct GameInput
{
    bool up, right, down, left;
};

struct GameState
{
    struct {
        int x, y;
    } cat_pos;
    bool tile_map[TILE_MAP_HEIGHT][TILE_MAP_WIDTH];
};

struct GameColor
{
    u8 r;
    u8 g;
    u8 b;
};
