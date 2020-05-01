#ifndef MAPGROUND_H
#define MAPGROUND_H

#include "mapelement.cpp"

namespace Map {
    class MapGround : public MapElement
    {
    private:

    public:
        MapGround(int, int);
        QString typeName() const override {return "ground";}
        ~MapGround(){};
    };
}

#endif // MAPGROUND_H
