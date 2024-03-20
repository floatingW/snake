#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include "definition.hpp"

#include <vector>
#include <functional> // function
#include <random>

namespace game
{

    template<typename T>
    class Matrix
    {
        using Row = std::vector<T>;

    public:
        Matrix() = delete;
        Matrix(size_t row, size_t col) :
            row_(row), col_(col), data_(row, Row(col))
        {
        }

        void Set(size_t r, size_t c, T v)
        {
            data_.at(r).at(c) = v;
        }

        void Set(Position pos, T v)
        {
            data_.at(pos.y).at(pos.x) = v;
        }

        auto Get(Position pos) const
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

        [[nodiscard]] Position RandomPosition(T v)
        {
            std::vector<Position> all;
            ForEach([v, &all](auto element, auto r, auto c) {
                if (element == v)
                {
                    all.push_back(Position(c, r));
                }
            });

            std::uniform_int_distribution<> dis(0, all.size() - 1);
            return all[dis(random_gen_)];
        }

        [[nodiscard]] bool IsOutOfBoundary(Position pos) const
        {
            return pos.x < 0 || pos.x >= static_cast<decltype(pos.x)>(col_) || pos.y < 0 || pos.y >= static_cast<decltype(pos.y)>(row_);
        }

        void SetSeed(std::mt19937::result_type seed)
        {
            random_gen_.seed(seed);
        }

    private:
        size_t row_{};
        size_t col_{};
        std::vector<Row> data_;
        std::mt19937 random_gen_{ std::mt19937::result_type{ 42 } };
    };
}

#endif // __MATRIX_HPP__
