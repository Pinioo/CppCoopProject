#ifndef MAPFINISH_H
#define MAPFINISH_H

#include "map.h"

namespace Map {

    class MapFinish : public MapElement
    {
    private:
        int _playerID;
        QColor _color;
    public:
        MapFinish(Map*, int, int, int, QColor);
        int playerID() const{return _playerID;}
        QColor color() const{return  _color;}
    };
}

#endif // MAPFINISH_H
