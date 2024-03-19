#ifndef __DEFINITION_HPP__
#define __DEFINITION_HPP__

#include <cassert>
#include <cstddef>

namespace definition
{

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    struct Position
    {
        Position(int x, int y) noexcept :
            x(x), y(y)
        {
        }
        int x{};
        int y{};

        Position Step(Direction dir) const noexcept
        {
            Position p(*this);
            switch (dir)
            {
            case Direction::Down:
                ++p.y;
                break;
            case Direction::Up:
                --p.y;
                break;
            case Direction::Left:
                --p.x;
                break;
            case Direction::Right:
                ++p.x;
                break;
            default:
            {
                assert(false);
            }
            }

            return p;
        }

        auto operator==(const Position& other) const noexcept
        {
            return other.x == x && other.y == y;
        }
    };

    enum class Tile
    {
        Open,
        SnakeUp,
        SnakeDown,
        SnakeLeft,
        SnakeRight,
        Food
    };

    class PlayerID
    {
    public:
        explicit PlayerID(int id) :
            id_(id)
        {
        }

        explicit PlayerID(std::size_t id) :
            id_(static_cast<int>(id))
        {
        }

        operator size_t()
        {
            return id_;
        }

    private:
        int id_;
    };

}

#endif // __DEFINITION_HPP__
