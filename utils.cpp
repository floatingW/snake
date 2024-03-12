#include "utils.hpp"

void utils::ResetScreen() noexcept
{
    endwin();
}

void utils::InitScreen() noexcept
{
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    nodelay(stdscr, true);
    curs_set(0);
}

void utils::Refresh() noexcept
{
    refresh();
}
