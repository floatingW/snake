#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <ncurses.h>

namespace utils
{

    // some ncurses routine
    void InitScreen() noexcept;
    void ResetScreen() noexcept;
    void Refresh() noexcept;

    template<typename... Ts>
    void MoveCursorPrintf(int y, int x, const char* fmt, Ts... args) noexcept
    {
        mvprintw(y, x, fmt, args...);
    }

}

#endif // __UTILS_HPP__
