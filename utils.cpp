#include "utils.hpp"

void utils::ResetScreen()
{
    endwin();
}

void utils::InitScreen()
{
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    nodelay(stdscr, true);
    curs_set(0);
}

void utils::Refresh()
{
    refresh();
}
