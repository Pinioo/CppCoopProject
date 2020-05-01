#ifndef MAPFINISH_H
#define MAPFINISH_H

#include "map.h"

namespace Map {

    class MapFinish : public MapElement
    {
    private:
        int _playerID;
        Color _color;
    public:
        MapFinish(int, int, int, Color);
        int playerID() const{return _playerID;}
        Color color() const{return  _color;}

        QString typeName() const override {return "finish";}

        QJsonObject toJsonObject() const override;
    };
}

#endif // MAPFINISH_H
