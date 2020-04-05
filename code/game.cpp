#include "types.h"

u8 color_offset = 0;

void game_update_and_render(u32* pixels, int width, int height)
{
    int num_pixels = width * height;
    u8 red = color_offset;
    u8 green = 64 + color_offset;
    u8 blue = 128 + color_offset;
    for (int pixel_index = 0; pixel_index < num_pixels; pixel_index++)
    {
        u32 pixel_value = (red << 16) | (green << 8) | blue;
        pixels[pixel_index] = pixel_value;
    }
    color_offset++;
}