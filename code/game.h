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
    u8 color_offset;
    int movement_offset;
};
