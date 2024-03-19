#include <optional>
#include <cassert>
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

}

auto main() -> int
{
    auto game_width{ 100 };
    auto game_height{ 30 };

    // init console
    auto game_world_size = utils::InitScreen(game_width, game_height, game_config::MIN_CONSOLE_WIDTH, game_config::MIN_CONSOLE_HEIGHT);
    if (!game_world_size)
    {
        nodelay(stdscr, false);
        getch();
        utils::ResetScreen();
        return 0;
    }

    auto [game_world_width, game_world_height] = *game_world_size;

    utils::Refresh();

    game_object::PlayGround pg{ game_world_height, game_world_width };
    pg.PutRandomFood();

    // initial direction
    Direction dir{ Direction::Left };

    {
        // initial pos
        int initial_pos_x = game_world_width / 2;
        int initial_pos_y = game_world_height / 2;

        pg.AddSnake(Position{ initial_pos_x, initial_pos_y }, dir);
    }

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
                dir = *optional_dir;
                pg.SetSnakeDir(definition::PlayerID{ 0 }, dir);
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
