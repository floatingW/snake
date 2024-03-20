#include "gameobject.hpp"
#include <cstddef>
#include <cassert>

auto game_object::PlayGround::AddSnake(definition::Position pos, definition::Direction dir) noexcept -> definition::PlayerID
{
    auto tile = utils::DirectionToSnakeTile(dir);
    assert(tile);
    Set(pos, *tile);
    utils::Draw(pos, tile.value());
    snakes_.emplace_back(pos, dir);
    return definition::PlayerID{ snakes_.size() };
}

auto game_object::PlayGround::SetSnakeDir(definition::PlayerID id, definition::Direction dir) noexcept -> bool
{
    auto* snake = GetSnake(id);
    if (snake != nullptr)
    {
        return snake->SetDir(dir);
    }

    return false;
}

auto game_object::PlayGround::Step() -> bool
{
    using namespace definition;
    size_t dead_num{ 0 };
    for (auto& snake_go : snakes_)
    {
        if (snake_go.Dead())
        {
            ++dead_num;
            continue;
        }

        Position next_pos = snake_go.NextFrontPosition();

        if (IsOutOfBoundary(next_pos))
        {
            snake_go.Die();
            ++dead_num;
            continue;
        }

        {
            auto snake_front_tile = utils::DirectionToSnakeTile(snake_go.Dir());
            assert(snake_front_tile.has_value());

            if (Get(next_pos) == Tile::Food)
            {
                snake_go.Expand(next_pos);
                Set(next_pos, snake_front_tile.value());
                utils::Draw(next_pos, snake_front_tile.value());
                // next food
                PutRandomFood();
            }
            else
            {
                // An open tile
                Set(next_pos, snake_front_tile.value());
                utils::Draw(next_pos, snake_front_tile.value());

                auto snake_back = snake_go.Forward(next_pos);
                Set(snake_back, Tile::Open);
                utils::Draw(snake_back, Tile::Open);
            }
        }
    }

    return dead_num != snakes_.size();
}
