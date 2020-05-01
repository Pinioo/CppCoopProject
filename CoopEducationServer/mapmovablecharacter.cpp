#include "mapmovablecharacter.h"

Map::MapMovableCharacter::MapMovableCharacter(int x, int y, int id, Color color) :
    MapElementMovable(x, y),
    _playerID(id),
    _color(color)
{

}

QJsonObject Map::MapMovableCharacter::toJsonObject() const
{
    QJsonObject elementObject = MapElement::toJsonObject();
    elementObject.insert("id", _playerID);
    elementObject.insert("r", color().red);
    elementObject.insert("g", color().green);
    elementObject.insert("b", color().blue);
    return elementObject;
}
