struct TileCoord
{
    int x, y;
};

struct GameDisplay
{
    u32 *pixels;
    int width, height;
};

struct GameInput
{
    bool up, right, down, left;
};

struct Food
{
    TileCoord pos;
    bool exists;
};

struct GameState
{
    f64 time;
    f64 delta_time;
    TileCoord cat_pos;
    Food food[8];
    size_t num_food;
    bool tile_map[TILE_MAP_HEIGHT][TILE_MAP_WIDTH];
};

struct GameColor
{
    u8 r;
    u8 g;
    u8 b;
};
