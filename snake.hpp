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
        Snake(Position p, Direction dir) :
            dir_(dir)
        {
            data_.emplace_front(p);
        }

        Snake(int x, int y, Direction dir) :
            dir_(dir)
        {
            data_.emplace_front(x, y);
        }

        Position FrontPosition() const
        {
            return data_.front();
        }

        Position NextFrontPosition() const
        {
            return FrontPosition().Step(dir_);
        }

        void Expand(Position pos)
        {
            data_.push_front(pos);
        }

        [[nodiscard]] Position Forward(Position pos)
        {
            Expand(pos);
            auto back = data_.back();
            data_.pop_back();
            return back;
        }

        [[nodiscard]] auto SetDir(Direction dir)
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
