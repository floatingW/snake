#include "utils.hpp"

void utils::ResetScreen() noexcept
{
    endwin();
}

auto utils::InitScreen() noexcept -> std::pair<int, int>
{
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    nodelay(stdscr, true);
    curs_set(0);

    int console_size_y{};
    int console_size_x{};
    getmaxyx(stdscr, console_size_y, console_size_x);

    return { console_size_x, console_size_y };
}

void utils::Refresh() noexcept
{
    refresh();
}
