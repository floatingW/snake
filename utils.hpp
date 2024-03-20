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
        auto InitScreen(int game_width, int game_height, int min_width, int min_height) -> std::optional<std::pair<int, int>>;
        void ResetScreen();
        void Refresh();

        template<typename... Ts>
        void MoveCursorPrintf(Ts... args)
        {
            mvprintw(args...);
        }

        void PrintExitMessage(const std::string& message);

        auto ReversedDirection(Direction dir) -> std::optional<Direction>;

        auto DirectionToSnakeTile(Direction dir) -> std::optional<Tile>;

        inline auto TickToMilliSeconds(unsigned tick_per_sec)
        {
            using namespace std::literals::chrono_literals;
            return 1.0 / tick_per_sec * 1000ms;
        }

        auto TileToChar(Tile v);

        void Draw(int x, int y, Tile v);

        void Draw(Position pos, Tile v);

        auto KeyToDirection(int ch) -> std::optional<Direction>;

        void Wait();
    }
}

#endif // __UTILS_HPP__
