#include <optional>
#include <thread> // sleepfor()

#include <ncurses.h>

#include "definition.hpp"
#include "playground.hpp"
#include "utils.hpp"

namespace game_config
{
    constexpr auto MIN_CONSOLE_WIDTH{ 90 };
    constexpr auto MIN_CONSOLE_HEIGHT{ 20 };
}

namespace detail
{
    auto KeyToDirection(int ch) noexcept -> std::optional<game::Direction>
    {
        switch (ch)
        {
        case 'w':
            return game::Direction::Up;
        case 's':
            return game::Direction::Down;
        case 'a':
            return game::Direction::Left;
        case 'd':
            return game::Direction::Right;
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
        game::utils::ResetScreen();
    }

}

auto main() -> int
{
    auto game_width{ 100 };
    auto game_height{ 30 };

    // init console
    auto game_world_size = game::utils::InitScreen(game_width, game_height, game_config::MIN_CONSOLE_WIDTH, game_config::MIN_CONSOLE_HEIGHT);
    if (!game_world_size)
    {
        detail::Wait();
        return 0;
    }

    game::utils::Refresh();

    auto [game_world_width, game_world_height] = *game_world_size;
    game::PlayGround pg{ game_world_height, game_world_width };
    pg.PutRandomFood();
    pg.AddSnake(game::Position{ game_world_width / 2, game_world_height / 2 }, game::Direction::Left);

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
                pg.SetSnakeDir(game::PlayerID{ 0 }, *optional_dir);
            }
        }

        if (!pg.Step())
        {
            break;
        }

        game::utils::Refresh();
        std::this_thread::sleep_for(game::utils::TickToMilliSeconds(2));
    }

    game::utils::ResetScreen();
    return 0;
}
