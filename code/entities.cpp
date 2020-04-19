Food *make_food(int x, int y)
{
    Food *result = new Food();
    result->type = ENTITY_FOOD;
    result->pos = (TileCoord){x, y};
    result->exists = true;
    return result;
}

Cat *make_cat(int x, int y, int life)
{
    Cat *result = new Cat();
    result->type = ENTITY_CAT;
    result->pos = (TileCoord){x, y};
    result->life = life;
    return result;
}

Entity *make_goal(int x, int y)
{
    Entity *result = new Entity();
    result->type = ENTITY_GOAL;
    result->pos = (TileCoord){x, y};
    return result;
}

void add_entity(GameState *state, Entity *entity)
{
    // TODO: assert(state->entity_count < MAX_ENTITY_COUNT)
    state->entities[state->entity_count] = entity;
    state->entity_count++;
}