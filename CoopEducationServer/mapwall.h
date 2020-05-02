#ifndef MAPWALL_H
#define MAPWALL_H

#include "map.h"

namespace Map {
    class MapWall : public MapElement
    {
    public:
        MapWall(int x,int y) : MapElement(x,y){};
        QString typeName() const override {return "wall";}
        ~MapWall(){};
    };
}

#endif // MAPWALL_H
