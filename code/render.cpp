#include <math.h>

#define PI 3.14159265359
#define RAD_PER_DEG 0.01745329251

struct ColorF3
{
    // NOTE: all from 0.0 to 1.0
    float r, g, b;
};

/**
 * amount is from 0.0 to 1.0
 */
ColorF3 color_lerp(ColorF3 from, ColorF3 to, float amount)
{
    float r_add = (to.r - from.r) * amount;
    float g_add = (to.g - from.g) * amount;
    float b_add = (to.b - from.b) * amount;
    
    ColorF3 result = {
        from.r + r_add,
        from.g + g_add,
        from.b + b_add,
    };
    return result;
}

u32 colorf3_to_u32(ColorF3 input)
{
    u8 r_byte = input.r * 255; // TODO cast to int?
    u8 g_byte = input.g * 255;
    u8 b_byte = input.b * 255;

    u32 result = (r_byte << 16) | (g_byte << 8) | b_byte;
    return result;
}

ColorF3 water_bg_normal   = {0.06666666666, 0.13333333333, 0.6 };
ColorF3 water_wave_normal = {0.4,           0.46666666666, 0.93333333333};
ColorF3 water_bg_won      = {0.0,           1.0,           1.0};
ColorF3 water_wave_won    = {1.0,           0.0,           1.0};

// TODO will time overflow in any realistic amount of run time?
//  would it matter?
u32 water_pixel(GameState *state, int x, int y, float time)
{
    static int move_range = 33;

    ColorF3 bg_color = !state->won ? water_bg_normal : water_bg_won;
    ColorF3 wave_color = !state->won ? water_wave_normal : water_wave_won;

    if (state->won)
    {
        time *= 5;
    }

    float y_phase = fabsf(sinf(RAD_PER_DEG * (y * 1.6 - time * 45)));
    float wave_align_y = y_phase * .5 + fabsf(sinf(RAD_PER_DEG * (y + time * 90))) * .5;

    float x_phase = fabsf(sinf(RAD_PER_DEG * (x * 1.3 - time * 25)));
    float wave_align_x = x_phase * fabsf(sinf(RAD_PER_DEG * (x + time * 50)));

    ColorF3 color = color_lerp(bg_color, wave_color, wave_align_x * wave_align_y);
    u32 result = colorf3_to_u32(color);
    return result;
}
