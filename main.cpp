#include <cstddef>
#include <deque>
#include <functional>
#include <iostream> // cout
#include <ostream>
#include <queue>
#include <unistd.h> // fd
#include <sys/ioctl.h> // ioctl()
#include <utility> // make_pair()
#include <vector>
#include <array>
#include <cassert>
#include <random>

namespace detail
{
    void SetCursorVisibility(bool visible)
    {
        if (visible)
        {
            std::cout << "\033[?25h";
        }
        else
        {
            std::cout << "\033[?25l";
        }
    }

    auto GetConsoleWinSize()
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return std::make_pair(w.ws_row, w.ws_col);
    }
}

enum class Tile
{
    Open,
    Snake,
    Food
};

namespace detail
{
    auto TileToChar(Tile v)
    {
        switch (v)
        {
        case Tile::Open:
            return 'O';
        case Tile::Snake:
            return 'S';
        case Tile::Food:
            return 'F';
        default:
            assert(false);
            return ' ';
        }
    }
}

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

template<typename T>
class Matrix
{
    using Row = std::vector<T>;

public:
    Matrix() = delete;
    Matrix(size_t row, size_t col) :
        data_(row, Row(col, Tile::Open))
    {
    }

    void Set(size_t r, size_t c, T v)
    {
        data_.at(r).at(c) = v;
    }

    void Set(Position pos, Tile v)
    {
        data_.at(pos.y).at(pos.x) = v;
    }

    auto Get(Position pos)
    {
        return data_.at(pos.y).at(pos.x);
    }

    void ForEach(std::function<void(Tile&, size_t, size_t)> f)
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

    // Debug
    void Print() const
    {
        for (auto r : data_)
        {
            for (auto v : r)
            {
                std::cout << detail::TileToChar(v);
            }
            std::cout << std::endl;
        }
    }

    Position RandomTile(T v)
    {
        std::vector<Position> all;
        ForEach([v, &all](auto element, auto r, auto c) {
            if (element == v)
            {
                all.push_back(Position(c, r));
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

namespace detail
{

    auto Draw(size_t x, size_t y, Tile v)
    {
        std::cout << "\033[" << y << ";" << x << "H" << TileToChar(v) << std::flush;
    }

    auto Draw(Position pos, Tile v)
    {
        return Draw(pos.x, pos.y, v);
    }

}


int main()
{
    // init console
    auto [console_size_y, console_size_x] = detail::GetConsoleWinSize();
    std::cout << "console size:" << console_size_y << ":" << console_size_x << std::endl;
    detail::SetCursorVisibility(false);

    // map
    Matrix<Tile> map(console_size_y, console_size_x);

    // initial pos
    size_t initial_pos_x = console_size_x / 2;
    size_t initial_pos_y = console_size_y / 2;
    map.Set(initial_pos_y, initial_pos_x, Tile::Snake);
    detail::Draw(initial_pos_x, initial_pos_y, Tile::Snake);

    // random food
    auto random_pos = map.RandomTile(Tile::Open);
    map.Set(random_pos, Tile::Food);
    detail::Draw(random_pos, Tile::Food);

    std::deque<Position> snake;
    snake.push_front(Position(initial_pos_x, initial_pos_y));

    // initial direction

    return 0;
}
