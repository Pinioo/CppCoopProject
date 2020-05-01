#ifndef MAPGROUND_H
#define MAPGROUND_H

#include "mapelement.cpp"
#include "q_includes.h"


namespace Map {
    class MapGround : public MapElement
    {
    private:

    public:
        MapGround(Map*, int, int);
    };
}

#endif // MAPGROUND_H
