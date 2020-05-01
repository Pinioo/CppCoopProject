#include "mapground.h"

Map::MapGround::MapGround(Map* map, int x, int y) : MapElement(map, x,y) {
    this->_mayBeOccupied = true;
}


