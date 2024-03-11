#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

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
        Matrix(size_t row, size_t col) :
            data_(row, Row(col))
        {
        }

        void Set(size_t r, size_t c, T v)
        {
            data_.at(r).at(c) = v;
        }

        void Set(definition::Position pos, T v)
        {
            data_.at(pos.y).at(pos.x) = v;
        }

        auto Get(definition::Position pos)
        {
            return data_.at(pos.y).at(pos.x);
        }

        void ForEach(std::function<void(T, size_t, size_t)> f) const
        {
            for (decltype(data_.size()) r = 0; r < data_.size(); ++r)
            {
                auto row_vec = data_[r];
                for (decltype(row_vec.size()) c = 0; c < row_vec.size(); ++c)
                {
                    f(row_vec[c], r, c);
                }
            }
        }

        definition::Position RandomPosition(T v) const
        {
            std::vector<definition::Position> all;
            ForEach([v, &all](auto element, auto r, auto c) {
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

    private:
        std::vector<Row> data_;
    };
}

#endif // __GAMEOBJECT_HPP__
