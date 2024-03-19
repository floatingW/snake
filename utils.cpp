#include "utils.hpp"
#include "definition.hpp"
#include <ncurses.h>
#include <optional>
#include <format>
#include <utility>

namespace detail
{
    struct WinBorder
    {
        chtype ls, rs, ts, bs,
            tl, tr, bl, br;
    };

    struct Win
    {
        int start_x, start_y;
        int height, width;
        WinBorder border;
    };

    auto InitWinParams(auto width, auto height)
    {
        Win win;
        win.height = height;
        win.width = width;
        win.start_y = 0;
        win.start_x = 0;

        win.border.ls = '|';
        win.border.rs = '|';
        win.border.ts = '-';
        win.border.bs = '-';
        win.border.tl = '+';
        win.border.tr = '+';
        win.border.bl = '+';
        win.border.br = '+';

        return win;
    }

    void CreateBox(Win* p_win)
    {
        int x = p_win->start_x;
        int y = p_win->start_y;
        int w = p_win->width;
        int h = p_win->height;

        mvaddch(y, x, p_win->border.tl);
        mvaddch(y, x + w - 1, p_win->border.tr);
        mvaddch(y + h - 1, x, p_win->border.bl);
        mvaddch(y + h - 1, x + w - 1, p_win->border.br);

        mvhline(y, x + 1, p_win->border.ts, w - 2);
        mvhline(y + h - 1, x + 1, p_win->border.bs, w - 2);
        mvvline(y + 1, x, p_win->border.ls, h - 2);
        mvvline(y + 1, x + w - 1, p_win->border.rs, h - 2);
    }

    auto SatisfiedConsoleSize(int width, int height, int min_width, int min_height) noexcept -> bool
    {
        if (width >= min_width && height >= min_height)
        {
            return true;
        }

        utils::PrintExitMessage(std::format("console size too small, current: {}:{}, required: {}:{}",
                                            width,
                                            height,
                                            min_width,
                                            min_height));
        return false;
    }
}

void utils::ResetScreen() noexcept
{
    endwin();
}

auto utils::InitScreen(int game_width, int game_height, int min_width, int min_height) noexcept -> std::optional<std::pair<int, int>>
{
    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    nodelay(stdscr, true);
    curs_set(0);

    {
        int console_height{};
        int console_width{};
        getmaxyx(stdscr, console_height, console_width);

        // game size >= min size
        // console size >= min size
        // console size >= game size + 2 (for border)
        auto border_width = game_width + 2;
        auto border_height = game_height + 2;
        if (!detail::SatisfiedConsoleSize(game_width, game_height, min_width, min_height) ||
            !detail::SatisfiedConsoleSize(console_width, console_height, min_width, min_height) ||
            !detail::SatisfiedConsoleSize(console_width, console_height, border_width, border_height))
        {
            return {};
        }

        auto win = detail::InitWinParams(border_width, border_height);
        detail::CreateBox(&win);
    }

    refresh();

    return { std::make_pair(game_width, game_height) };
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

auto utils::DirectionToSnakeTile(definition::Direction dir) noexcept -> std::optional<definition::Tile>
{
    using namespace definition;
    switch (dir)
    {
    case Direction::Up:
        return Tile::SnakeUp;
    case Direction::Down:
        return Tile::SnakeDown;
    case Direction::Left:
        return Tile::SnakeLeft;
    case Direction::Right:
        return Tile::SnakeRight;
    default:
    {
        return {};
    }
    }
}
