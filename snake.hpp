#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include "definition.hpp"
#include "utils.hpp"

#include <deque>

namespace game
{
    class Snake
    {
    public:
        Snake(Position p, Direction dir) noexcept :
            dir_(dir)
        {
            data_.emplace_front(p);
        }

        Snake(int x, int y, Direction dir) noexcept :
            dir_(dir)
        {
            data_.emplace_front(x, y);
        }

        Position FrontPosition() const noexcept
        {
            return data_.front();
        }

        Position NextFrontPosition() const noexcept
        {
            return FrontPosition().Step(dir_);
        }

        void Expand(Position pos) noexcept
        {
            data_.push_front(pos);
        }

        [[nodiscard]] Position Forward(Position pos) noexcept
        {
            Expand(pos);
            auto back = data_.back();
            data_.pop_back();
            return back;
        }

        [[nodiscard]] auto SetDir(Direction dir) noexcept
        {
            if (data_.size() > 1 && dir == utils::ReversedDirection(dir_))
            {
                return false;
            }

            dir_ = dir;
            return true;
        }

        Direction Dir() const { return dir_; }
        auto Dead() const { return dead_; }
        void Die() { dead_ = true; }

    private:
        std::deque<Position> data_;
        Direction dir_{ Direction::Left };
        bool dead_{ false };
    };
}

#endif // __SNAKE_HPP__
