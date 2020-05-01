#ifndef MAPWALL_H
#define MAPWALL_H

#include "map.h"

namespace Map {
    class MapWall : public MapElement
    {
    public:
        MapWall(int,int);
        QString typeName() const override {return "wall";}
        ~MapWall(){};
    };
}

#endif // MAPWALL_H
