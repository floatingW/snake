#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <ncurses.h>
#include <string>
#include <utility>

namespace utils
{

    // some ncurses routine
    std::pair<int, int> InitScreen() noexcept;
    void ResetScreen() noexcept;
    void Refresh() noexcept;

    template<typename... Ts>
    void MoveCursorPrintf(Ts... args) noexcept
    {
        mvprintw(args...);
    }

    void PrintExitMessage(const std::string& message) noexcept;
}

#endif // __UTILS_HPP__
