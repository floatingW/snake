#include "utils.hpp"
#include "definition.hpp"
#include <optional>

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

void utils::PrintExitMessage(const std::string& message) noexcept
{
    utils::MoveCursorPrintf(0, 0, "%s", message.c_str());
    utils::Refresh();

    nodelay(stdscr, false);
    getch();
}

auto utils::ReversedDirection(definition::Direction dir) noexcept -> std::optional<definition::Direction>
{
    switch (dir)
    {
    case definition::Direction::Up:
        return definition::Direction::Down;
    case definition::Direction::Down:
        return definition::Direction::Up;
    case definition::Direction::Left:
        return definition::Direction::Right;
    case definition::Direction::Right:
        return definition::Direction::Left;
    default:
        return {};
    }
}
