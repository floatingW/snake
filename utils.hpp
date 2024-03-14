#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <ncurses.h>
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

}

#endif // __UTILS_HPP__
