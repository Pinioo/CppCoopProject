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
        Color _color;

    public:
        MapMovableCharacter(int, int, int, Color);
        QString typeName() const override {return "character";}

        Direction direction() const {return _direction;}

        int playerID() const {return _playerID;}
        Color color() const {return _color;}

        QJsonObject toJsonObject() const override;

        ~MapMovableCharacter(){};
    };
}

#endif // MAPCHARACTER_H
