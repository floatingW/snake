#include <cstddef>
#include <deque>
#include <iostream> // cout
#include <ostream>
#include <queue>
#include <unistd.h> // fd
#include <sys/ioctl.h> // ioctl()
#include <utility> // make_pair()
#include <vector>
#include <array>

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

std::array<char, sizeof(Tile)> TileChar{
    { 'O', 'S', 'F' }
};

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

    // Debug
    void Print() const
    {
        for (auto r : data_)
        {
            for (auto v : r)
            {
                std::cout << TileChar.at(static_cast<size_t>(v));
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<Row> data_;
};

struct Position
{
    Position() = default;
    Position(size_t x, size_t y) :
        x(x), y(y)
    {
    }
    size_t x{};
    size_t y{};
};

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

    std::deque<Position> snake;
    snake.push_front(Position(initial_pos_x, initial_pos_y));

    // initial direction

    return 0;
}
