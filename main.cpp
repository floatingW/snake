#include <deque>
#include <optional>
#include <cassert>
#include <thread> // sleepfor()

#include "definition.hpp"
#include "gameobject.hpp"
#include "utils.hpp"

using Tile = definition::Tile;
using Position = definition::Position;
using Direction = definition::Direction;
using Matrix = game_object::Matrix<Tile>;

namespace detail
{
    auto TickToMilliSeconds(unsigned tick_per_sec) noexcept
    {
        using namespace std::chrono_literals;
        return 1.0 / tick_per_sec * 1000ms;
    }

    auto TileToChar(Tile v) noexcept
    {
        switch (v)
        {
        case Tile::Open:
            return ' ';
        case Tile::SnakeUp:
            return '^';
        case Tile::SnakeDown:
            return 'v';
        case Tile::SnakeLeft:
            return '<';
        case Tile::SnakeRight:
            return '>';
        case Tile::Food:
            return '@';
        default:
            assert(false);
            return '#';
        }
    }

    auto Draw(int x, int y, Tile v) noexcept
    {
        utils::MoveCursorPrintf(y, x, "%c", detail::TileToChar(v));
    }

    auto Draw(Position pos, Tile v) noexcept
    {
        return Draw(pos.x, pos.y, v);
    }

    auto KeyToDirection(int ch) noexcept -> std::optional<Direction>
    {
        switch (ch)
        {
        case 'w':
            return Direction::Up;
        case 's':
            return Direction::Down;
        case 'a':
            return Direction::Left;
        case 'd':
            return Direction::Right;
        default:
        {
            return {};
        }
        }
    }

    auto DirectionToSnakeTile(Direction dir) noexcept -> std::optional<Tile>
    {
        switch (dir)
        {
        case Direction::Up:
            return Tile::SnakeUp;
        case Direction::Down:
            return Tile::SnakeDown;
        case Direction::Left:
            return Tile::SnakeLeft;
        case Direction::Right:
            return Tile::SnakeRight;
        default:
        {
            return {};
        }
        }
    }

}

auto main() -> int
{
    // init console
    auto [console_size_x, console_size_y] = utils::InitScreen();
    utils::Refresh();

    game_object::PlayGround pg{ console_size_y, console_size_x };

    // initial direction
    Direction dir{ Direction::Left };

    // initial pos
    int initial_pos_x = console_size_x / 2;
    int initial_pos_y = console_size_y / 2;

    {
        auto tile = detail::DirectionToSnakeTile(dir);
        assert(tile.has_value());
        pg.Set(initial_pos_y, initial_pos_x, tile.value());
        detail::Draw(initial_pos_x, initial_pos_y, tile.value());
    }

    {
        // random food
        detail::Draw(pg.PutRandomFood(), Tile::Food);
    }

    std::deque<Position> snake;
    snake.emplace_front(initial_pos_x, initial_pos_y);

    while (true)
    {
        {
            auto ch = getch();
            if (ch == 'q')
            {
                break;
            }

            auto optional_dir = detail::KeyToDirection(ch);
            if (optional_dir.has_value())
            {
                dir = *optional_dir;
            }
        }

        Position current_pos = snake.front();
        Position next_pos = current_pos.Step(dir);

        if (pg.IsOutOfBoundary(next_pos))
        {
            break;
        }

        // step forward
        snake.push_front(next_pos);

        {
            auto snake_front_tile = detail::DirectionToSnakeTile(dir);
            assert(snake_front_tile.has_value());

            if (pg.Get(next_pos) == Tile::Food)
            {
                pg.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());
                // next food
                pg.PutRandomFood();
            }
            else
            {
                // An open tile
                pg.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());
                pg.Set(snake.back(), Tile::Open);
                detail::Draw(snake.back(), Tile::Open);

                snake.pop_back();
            }
        }

        utils::Refresh();
        std::this_thread::sleep_for(detail::TickToMilliSeconds(2));
    }

    utils::ResetScreen();
    return 0;
}
