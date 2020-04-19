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

enum EntityType
{
    ENTITY_CAT,
    ENTITY_FOOD,
    ENTITY_GOAL,
};

struct Entity
{
    TileCoord pos;
    EntityType type;
};

struct Cat : Entity
{
    int life;
};

struct Food : Entity
{
    bool exists;
};

struct GameState
{
    f64 time;
    f64 delta_time;
    Cat *cat;
    Entity *goal;
    bool won;
    Entity *entities[MAX_ENTITY_COUNT];
    size_t entity_count;
    bool tile_map[TILE_MAP_HEIGHT][TILE_MAP_WIDTH];
};
