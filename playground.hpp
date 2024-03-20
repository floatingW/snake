#ifndef __PLAYGROUND_HPP__
#define __PLAYGROUND_HPP__

#include "utils.hpp"
#include "matrix.hpp"
#include "snake.hpp"

#include <vector>

namespace game
{
    class PlayGround
    {
    public:
        template<typename... Ts>
        PlayGround(Ts... args) noexcept :
            map_(args...)
        {
        }

        template<typename... Ts>
        void Set(Ts... args) noexcept
        {
            map_.Set(args...);
        }

        template<typename... Ts>
        auto Get(Ts... args) const noexcept
        {
            return map_.Get(args...);
        }

        void PutRandomFood() noexcept
        {
            auto random_pos = map_.RandomPosition(Tile::Open);
            map_.Set(random_pos, Tile::Food);
            utils::Draw(random_pos, Tile::Food);
        }

        [[nodiscard]] bool IsOutOfBoundary(Position pos) const noexcept
        {
            return map_.IsOutOfBoundary(pos);
        }

        auto AddSnake(Position pos, Direction dir) noexcept -> PlayerID;
        auto SetSnakeDir(PlayerID id, Direction dir) noexcept -> bool;
        auto Step() -> bool;

    private:
        Matrix<Tile> map_;
        std::vector<Snake> snakes_;

        auto GetSnake(PlayerID id) noexcept -> Snake* { return GetSnakeImpl(*this, id); }
        inline auto GetSnake(PlayerID id) const noexcept -> const Snake* { return GetSnakeImpl(*this, id); }

        template<typename T>
        static auto GetSnakeImpl(T& t, PlayerID id) noexcept -> decltype(&t.snakes_[0])
        {
            auto index = static_cast<size_t>(id);
            if (index < t.snakes_.size())
            {
                return &t.snakes_[index];
            }

            return {};
        }
    };
}

#endif // __PLAYGROUND_HPP__
