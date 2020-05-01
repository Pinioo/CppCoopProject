#include "mappiston.h"

Map::MapPiston::MapPiston(Map* map, int x, int y, int b_x, int b_y) : MapElement(map, x, y){
    this->b_x = b_x;
    this->b_y = b_y;
}

void Map::MapPiston::updateAction(){
    _mayBeOccupied = (map->getElem(b_x, b_y)->occupyingElement() != nullptr);
}
