#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "definition.hpp"
#include <ncurses.h>
#include <optional>
#include <string>
#include <chrono>

namespace game
{
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

        auto ReversedDirection(Direction dir) noexcept -> std::optional<Direction>;

        auto DirectionToSnakeTile(Direction dir) noexcept -> std::optional<Tile>;

        inline auto TickToMilliSeconds(unsigned tick_per_sec) noexcept
        {
            using namespace std::literals::chrono_literals;
            return 1.0 / tick_per_sec * 1000ms;
        }

        auto TileToChar(Tile v) noexcept;

        void Draw(int x, int y, Tile v) noexcept;

        void Draw(Position pos, Tile v) noexcept;

        auto KeyToDirection(int ch) noexcept -> std::optional<Direction>;

        void Wait() noexcept;
    }
}

#endif // __UTILS_HPP__
