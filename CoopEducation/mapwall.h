#ifndef MAPWALL_H
#define MAPWALL_H

#include "map.h"

namespace Map {
    class MapWall : public MapElement
    {
    public:
        MapWall(Map*,int,int);
    };
}

#endif // MAPWALL_H
