#include <optional>
#include <cassert>
#include <thread> // sleepfor()

#include <ncurses.h>

#include "definition.hpp"
#include "gameobject.hpp"
#include "utils.hpp"

using Tile = definition::Tile;
using Position = definition::Position;
using Direction = definition::Direction;
using Matrix = game_object::Matrix<Tile>;

namespace game_config
{
    constexpr auto MIN_CONSOLE_WIDTH{ 90 };
    constexpr auto MIN_CONSOLE_HEIGHT{ 20 };
}

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
        mvaddch(y, x, detail::TileToChar(v));
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
    auto game_width{ 100 };
    auto game_height{ 30 };

    // init console
    auto game_world_size = utils::InitScreen(game_width, game_height, game_config::MIN_CONSOLE_WIDTH, game_config::MIN_CONSOLE_HEIGHT);
    if (!game_world_size)
    {
        nodelay(stdscr, false);
        getch();
        utils::ResetScreen();
        return 0;
    }

    auto [game_world_width, game_world_height] = *game_world_size;

    utils::Refresh();

    game_object::PlayGround pg{ game_world_height, game_world_width };

    // initial direction
    Direction dir{ Direction::Left };

    // initial pos
    int initial_pos_x = game_world_width / 2;
    int initial_pos_y = game_world_height / 2;

    {
        auto tile = detail::DirectionToSnakeTile(dir);
        assert(tile.has_value());
        pg.Set(initial_pos_y, initial_pos_x, tile.value());
        detail::Draw(initial_pos_x, initial_pos_y, tile.value());
    }

    // random food
    detail::Draw(pg.PutRandomFood(), Tile::Food);
    game_object::Snake snake_go{ initial_pos_x, initial_pos_y, dir };

    while (true)
    {
        {
            auto ch = getch();
            if (ch == 'q')
            {
                break;
            }

            auto optional_dir = detail::KeyToDirection(ch);
            if (optional_dir.has_value() && snake_go.SetDir(*optional_dir))
            {
                dir = *optional_dir;
            }
        }

        Position next_pos = snake_go.NextFrontPosition();

        if (pg.IsOutOfBoundary(next_pos))
        {
            break;
        }

        {
            auto snake_front_tile = detail::DirectionToSnakeTile(dir);
            assert(snake_front_tile.has_value());

            if (pg.Get(next_pos) == Tile::Food)
            {
                snake_go.Expand(next_pos);
                pg.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());
                // next food
                detail::Draw(pg.PutRandomFood(), definition::Tile::Food);
            }
            else
            {
                // An open tile
                pg.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());

                auto snake_back = snake_go.Forward(next_pos);
                pg.Set(snake_back, Tile::Open);
                detail::Draw(snake_back, Tile::Open);
            }
        }

        utils::Refresh();
        std::this_thread::sleep_for(detail::TickToMilliSeconds(2));
    }

    utils::ResetScreen();
    return 0;
}
