#include <optional>
#include <thread> // sleepfor()

#include <ncurses.h>

#include "definition.hpp"
#include "gameobject.hpp"
#include "utils.hpp"

using Tile = definition::Tile;
using Position = definition::Position;
using Direction = definition::Direction;
using Matrix = game_object::Matrix<Tile>;

namespace game_config
{
    constexpr auto MIN_CONSOLE_WIDTH{ 90 };
    constexpr auto MIN_CONSOLE_HEIGHT{ 20 };
}

namespace detail
{
    auto KeyToDirection(int ch) noexcept -> std::optional<Direction>
    {
        switch (ch)
        {
        case 'w':
            return Direction::Up;
        case 's':
            return Direction::Down;
        case 'a':
            return Direction::Left;
        case 'd':
            return Direction::Right;
        default:
        {
            return {};
        }
        }
    }

    void Wait() noexcept
    {
        nodelay(stdscr, false);
        getch();
        utils::ResetScreen();
    }

}

auto main() -> int
{
    auto game_width{ 100 };
    auto game_height{ 30 };

    // init console
    auto game_world_size = utils::InitScreen(game_width, game_height, game_config::MIN_CONSOLE_WIDTH, game_config::MIN_CONSOLE_HEIGHT);
    if (!game_world_size)
    {
        detail::Wait();
        return 0;
    }

    utils::Refresh();

    auto [game_world_width, game_world_height] = *game_world_size;
    game_object::PlayGround pg{ game_world_height, game_world_width };
    pg.PutRandomFood();
    pg.AddSnake(Position{ game_world_width / 2, game_world_height / 2 }, Direction::Left);

    while (true)
    {
        {
            auto ch = getch();
            if (ch == 'q')
            {
                break;
            }

            auto optional_dir = detail::KeyToDirection(ch);
            if (optional_dir)
            {
                pg.SetSnakeDir(definition::PlayerID{ 0 }, *optional_dir);
            }
        }

        if (!pg.Step())
        {
            break;
        }

        utils::Refresh();
        std::this_thread::sleep_for(utils::TickToMilliSeconds(2));
    }

    utils::ResetScreen();
    return 0;
}
