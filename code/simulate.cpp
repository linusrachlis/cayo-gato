void cat_move(GameState *state, int move_x, int move_y)
{
    if (state->won || state->cat->life <= 0) return;

    int new_x = state->cat->pos.x + move_x;
    int new_y = state->cat->pos.y + move_y;

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

    state->cat->life--;
    state->cat->pos.x = new_x;
    state->cat->pos.y = new_y;

    // Check entity contacts
    for (
        size_t entity_index = 0;
        entity_index < state->entity_count;
        entity_index++)
    {
        Entity *entity = state->entities[entity_index];
        if (entity->pos.x == state->cat->pos.x &&
            entity->pos.y == state->cat->pos.y)
        {
            // Contact!
            switch (entity->type)
            {
                case ENTITY_FOOD: {
                    Food *food = (Food *)entity;
                    // Take food if you're on the same tile
                    if (!food->exists)
                    {
                        continue;
                    }
                    state->cat->life += 5;
                    food->exists = false;
                } break;

                case ENTITY_GOAL: {
                    state->won = true;
                } break;
                
                default:
                    // The cat will of course contact himself. Duh.
                    break;
            }
        }
    }
}
