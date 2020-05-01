#include "mappistonbutton.h"

Map::MapPistonButton::MapPistonButton(Map* map, int x, int y, QColor color) : MapElement(map, x, y), _color(color)
{
    _mayBeOccupied = true;
}
