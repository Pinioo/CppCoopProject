#include "mappistonbutton.h"

Map::MapPistonButton::MapPistonButton(int x, int y, Color color) : MapElement(x, y), _color(color)
{
    _mayBeOccupied = true;
}

QJsonObject Map::MapPistonButton::toJsonObject() const
{
    QJsonObject elementObject = MapElement::toJsonObject();
    elementObject.insert("r", _color.red);
    elementObject.insert("g", _color.green);
    elementObject.insert("b", _color.blue);
    return elementObject;
}
