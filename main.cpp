#include <deque>
#include <format>
#include <iostream> // cout
#include <optional>
#include <cassert>
#include <chrono> // milliseconds
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
    auto TickToMilliSeconds(unsigned tick_per_sec)
    {
        return static_cast<unsigned>(1.0 / tick_per_sec * 1000);
    }

    auto TileToChar(Tile v)
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

    auto Draw(int x, int y, Tile v)
    {
        utils::MoveCursorPrintf(y, x, "%c", detail::TileToChar(v));
    }

    auto Draw(Position pos, Tile v)
    {
        return Draw(pos.x, pos.y, v);
    }

    auto IsOutOfBoundary(Position pos, int width, int height)
    {
        return pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height;
    }

    void PutRandomFood(Matrix& map)
    {
        auto random_pos = map.RandomPosition(Tile::Open);
        map.Set(random_pos, Tile::Food);
        detail::Draw(random_pos, Tile::Food);
    }

    std::optional<Direction> KeyToDirection(int ch)
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
        }

        return {};
    }

    std::optional<Tile> DirectionToSnakeTile(Direction dir)
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
        }

        return {};
    }

}

int main()
{
    // init console
    int console_size_y = 0;
    int console_size_x = 0;

    utils::InitScreen();
    {
        int row, col;
        getmaxyx(stdscr, row, col);
        console_size_x = col;
        console_size_y = row;
    }

    utils::Refresh();

    // map
    Matrix map(console_size_y, console_size_x);

    // initial direction
    Direction dir{ Direction::Left };

    // initial pos
    int initial_pos_x = console_size_x / 2;
    int initial_pos_y = console_size_y / 2;

    {
        auto tile = detail::DirectionToSnakeTile(dir);
        assert(tile.has_value());
        map.Set(initial_pos_y, initial_pos_x, tile.value());
        detail::Draw(initial_pos_x, initial_pos_y, tile.value());
    }

    // random food
    detail::PutRandomFood(map);

    std::deque<Position> snake;
    snake.push_front(Position(initial_pos_x, initial_pos_y));

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
                dir = optional_dir.value();
            }
        }

        Position current_pos = snake.front();
        Position next_pos = current_pos.Step(dir);

        if (detail::IsOutOfBoundary(next_pos, console_size_x, console_size_y))
        {
            break;
        }

        // step forward
        snake.push_front(next_pos);

        {
            auto snake_front_tile = detail::DirectionToSnakeTile(dir);
            assert(snake_front_tile.has_value());

            if (map.Get(next_pos) == Tile::Food)
            {
                map.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());
                // next food
                detail::PutRandomFood(map);
            }
            else
            {
                // An open tile
                map.Set(next_pos, snake_front_tile.value());
                detail::Draw(next_pos, snake_front_tile.value());
                map.Set(snake.back(), Tile::Open);
                detail::Draw(snake.back(), Tile::Open);

                snake.pop_back();
            }
        }

        utils::Refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(detail::TickToMilliSeconds(2)));
    }

    utils::ResetScreen();
    return 0;
}
