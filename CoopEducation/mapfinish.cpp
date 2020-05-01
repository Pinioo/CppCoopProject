#include "mapfinish.h"

Map::MapFinish::MapFinish(Map* map, int x, int y, int id, QColor color) : MapElement(map, x, y), _playerID(id), _color(color)
{
this->_mayBeOccupied = true;
}
