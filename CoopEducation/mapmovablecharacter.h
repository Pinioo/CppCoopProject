#ifndef MAPCHARACTER_H
#define MAPCHARACTER_H

#include "mapelementmovable.h"

namespace Map {
    class MapMovableCharacter : public MapElementMovable
    {
    public:
        enum class Direction{
            NORTH, EAST, SOUTH, WEST
        };
    private:

        const int unitVectors[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};

        Direction _direction = Direction::NORTH;

        int _playerID;

        QColor _color;

    public:
        MapMovableCharacter(Map*, int, int, int, QColor);

        void rotateLeft() {_direction = static_cast<Direction>((static_cast<int>(_direction) + 4 - 1) % 4); }
        void rotateRight() {_direction = static_cast<Direction>((static_cast<int>(_direction) + 4 + 1) % 4); }
        void stepForward() {move(_x + unitVectors[static_cast<int>(_direction)][0], _y + unitVectors[static_cast<int>(_direction)][1]);}
        void move(int, int) override;

        int playerID() const{return _playerID;}
        QColor color() const {return _color;}

        Direction direction() const {return _direction;}
    };
}

#endif // MAPCHARACTER_H
