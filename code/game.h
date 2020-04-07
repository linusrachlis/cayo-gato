#include "types.h"

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
};

struct GameColor
{
    u8 r;
    u8 g;
    u8 b;
};
