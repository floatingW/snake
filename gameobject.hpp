#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <cstddef>
#include <deque>
#include <vector>
#include <random>
#include <functional> // function

#include "definition.hpp"
#include "utils.hpp"

namespace game_object
{
    template<typename T>
    class Matrix
    {
        using Row = std::vector<T>;

    public:
        Matrix() = delete;
        Matrix(size_t row, size_t col) noexcept :
            row_(row), col_(col), data_(row, Row(col))
        {
        }

        void Set(size_t r, size_t c, T v) noexcept
        {
            data_.at(r).at(c) = v;
        }

        void Set(definition::Position pos, T v) noexcept
        {
            data_.at(pos.y).at(pos.x) = v;
        }

        auto Get(definition::Position pos) const noexcept
        {
            return data_.at(pos.y).at(pos.x);
        }

        void ForEach(std::function<void(T, size_t, size_t)> f) const
        {
            for (size_t r = 0; r < data_.size(); ++r)
            {
                auto row_vec = data_[r];
                for (size_t c = 0; c < row_vec.size(); ++c)
                {
                    f(row_vec[c], r, c);
                }
            }
        }

        [[nodiscard]] definition::Position RandomPosition(T v) noexcept
        {
            std::vector<definition::Position> all;
            ForEach([v, &all](auto element, auto r, auto c) noexcept {
                if (element == v)
                {
                    all.push_back(definition::Position(c, r));
                }
            });

            std::uniform_int_distribution<> dis(0, all.size() - 1);
            return all[dis(random_gen_)];
        }

        [[nodiscard]] bool IsOutOfBoundary(definition::Position pos) const noexcept
        {
            return pos.x < 0 || pos.x >= static_cast<decltype(pos.x)>(col_) || pos.y < 0 || pos.y >= static_cast<decltype(pos.y)>(row_);
        }

        void SetSeed(std::mt19937::result_type seed) noexcept
        {
            random_gen_.seed(seed);
        }

    private:
        size_t row_{};
        size_t col_{};
        std::vector<Row> data_;
        std::mt19937 random_gen_{ std::mt19937::result_type{ 42 } };
    };

    class Snake;
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

        [[nodiscard]] definition::Position PutRandomFood() noexcept
        {
            auto random_pos = map_.RandomPosition(Tile::Open);
            map_.Set(random_pos, Tile::Food);
            return random_pos;
        }

        [[nodiscard]] bool IsOutOfBoundary(definition::Position pos) const noexcept
        {
            return map_.IsOutOfBoundary(pos);
        }

    private:
        using Tile = definition::Tile;
        using Matrix = Matrix<definition::Tile>;
        Matrix map_;
    };

    class Snake
    {
    public:
        Snake(definition::Position p, definition::Direction dir) noexcept :
            dir_(dir)
        {
            data_.emplace_front(p);
        }

        Snake(int x, int y, definition::Direction dir) noexcept :
            dir_(dir)
        {
            data_.emplace_front(x, y);
        }

        definition::Position FrontPosition() const noexcept
        {
            return data_.front();
        }

        definition::Position NextFrontPosition() const noexcept
        {
            return FrontPosition().Step(dir_);
        }

        void Expand(definition::Position pos) noexcept
        {
            data_.push_front(pos);
        }

        [[nodiscard]] definition::Position Forward(definition::Position pos) noexcept
        {
            Expand(pos);
            auto back = data_.back();
            data_.pop_back();
            return back;
        }

        [[nodiscard]] auto SetDir(definition::Direction dir) noexcept
        {
            if (data_.size() > 1 && dir == utils::ReversedDirection(dir_))
            {
                return false;
            }

            dir_ = dir;
            return true;
        }

    private:
        using Position = definition::Position;
        using Direction = definition::Direction;

        std::deque<Position> data_;
        Direction dir_{ Direction::Left };
    };

}

#endif // __GAMEOBJECT_HPP__
