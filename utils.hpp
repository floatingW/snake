#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <cstdarg>

#include <ncurses.h>

#include "definition.hpp"

namespace utils
{

    // some ncurses routine
    void InitScreen();
    void ResetScreen();
    void Refresh();

    template<typename... Ts>
    void MoveCursorPrintf(int y, int x, const char* fmt, Ts... args)
    {
        mvprintw(y, x, fmt, args...);
    }

}

#endif // __UTILS_HPP__
