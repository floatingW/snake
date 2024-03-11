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
        case Tile::Snake:
            return '*';
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

    // initial pos
    int initial_pos_x = console_size_x / 2;
    int initial_pos_y = console_size_y / 2;
    map.Set(initial_pos_y, initial_pos_x, Tile::Snake);
    detail::Draw(initial_pos_x, initial_pos_y, Tile::Snake);

    // random food
    detail::PutRandomFood(map);

    std::deque<Position> snake;
    snake.push_front(Position(initial_pos_x, initial_pos_y));

    // initial direction
    Direction dir{ Direction::Left };

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

        if (map.Get(next_pos) == Tile::Food)
        {
            map.Set(next_pos, Tile::Snake);
            detail::Draw(next_pos, Tile::Snake);

            // next food
            detail::PutRandomFood(map);
        }
        else
        {
            // An open tile

            map.Set(next_pos, Tile::Snake);
            detail::Draw(next_pos, Tile::Snake);
            map.Set(snake.back(), Tile::Open);
            detail::Draw(snake.back(), Tile::Open);

            snake.pop_back();
        }

        utils::Refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(detail::TickToMilliSeconds(2)));
    }

    utils::ResetScreen();
    return 0;
}
