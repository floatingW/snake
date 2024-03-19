#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "definition.hpp"
#include <ncurses.h>
#include <optional>
#include <string>
#include <utility>
#include <chrono>

namespace utils
{

    // some ncurses routine
    auto InitScreen(int game_width, int game_height, int min_width, int min_height) noexcept -> std::optional<std::pair<int, int>>;
    void ResetScreen() noexcept;
    void Refresh() noexcept;

    template<typename... Ts>
    void MoveCursorPrintf(Ts... args) noexcept
    {
        mvprintw(args...);
    }

    void PrintExitMessage(const std::string& message) noexcept;

    auto ReversedDirection(definition::Direction dir) noexcept -> std::optional<definition::Direction>;

    auto DirectionToSnakeTile(definition::Direction dir) noexcept -> std::optional<definition::Tile>;

    inline auto TickToMilliSeconds(unsigned tick_per_sec) noexcept
    {
        using namespace std::literals::chrono_literals;
        return 1.0 / tick_per_sec * 1000ms;
    }

    inline auto TileToChar(definition::Tile v) noexcept
    {
        using namespace definition;
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

    inline auto Draw(int x, int y, definition::Tile v) noexcept
    {
        mvaddch(y, x, TileToChar(v));
    }

    inline auto Draw(definition::Position pos, definition::Tile v) noexcept
    {
        return Draw(pos.x, pos.y, v);
    }

}

#endif // __UTILS_HPP__
