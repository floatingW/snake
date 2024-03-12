#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <cstddef>
#include <vector>
#include <random>
#include <functional> // function

#include "definition.hpp"

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

        definition::Position RandomPosition(T v) const noexcept
        {
            std::vector<definition::Position> all;
            ForEach([v, &all](auto element, auto r, auto c) noexcept {
                if (element == v)
                {
                    all.push_back(definition::Position(c, r));
                }
            });

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, all.size() - 1);
            return all[dis(gen)];
        }

        bool IsOutOfBoundary(definition::Position pos) const noexcept
        {
            return pos.x < 0 || pos.x > static_cast<decltype(pos.x)>(col_) || pos.y < 0 || pos.y > static_cast<decltype(pos.y)>(row_);
        }

    private:
        size_t row_{};
        size_t col_{};
        std::vector<Row> data_;
    };

    class PlayGround
    {
    public:
        template<typename... Ts>
        PlayGround(Ts... args) noexcept :
            map(args...)
        {
        }

        template<typename... Ts>
        void Set(Ts... args) noexcept
        {
            map.Set(args...);
        }

        template<typename... Ts>
        auto Get(Ts... args) const noexcept
        {
            return map.Get(args...);
        }

        definition::Position PutRandomFood() noexcept
        {
            auto random_pos = map.RandomPosition(Tile::Open);
            map.Set(random_pos, Tile::Food);
            return random_pos;
        }

        bool IsOutOfBoundary(definition::Position pos) const noexcept
        {
            return map.IsOutOfBoundary(pos);
        }

    private:
        using Tile = definition::Tile;
        using Matrix = Matrix<definition::Tile>;
        Matrix map;
    };

}

#endif // __GAMEOBJECT_HPP__
