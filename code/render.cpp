// TODO will time overflow in any realistic amount of run time?
//  would it matter?
u32 water_pixel(GameState *state, int x, int y, f64 time)
{
    static int move_range = 33;

    u32 bg_color = !state->won ? 0x00112299 : 0x0000FFFF;
    u32 wave_color = !state->won ? 0x006677EE : 0x00FF00FF;

    if (state->won)
    {
        time *= 5;
    }

    f64 sin_time_y = sin(time + y);
    f64 sin_y = sin((f64)y);
    f64 sin_unit = (sin_time_y + sin_y) / 2.0;
    int offset_x = (int)(sin_unit * move_range) + x;
    bool wave = (((y + 6) % 33) == 0) &&
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
