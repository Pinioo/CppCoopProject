#include "mappiston.h"

Map::MapPiston::MapPiston(int x, int y, int b_x, int b_y) : MapElement(x, y)
{
    this->b_x = b_x;
    this->b_y = b_y;
}

QJsonObject Map::MapPiston::toJsonObject() const
{
    QJsonObject elementObject = MapElement::toJsonObject();
    elementObject.insert("buttonx", b_x);
    elementObject.insert("buttony", b_y);
    return elementObject;
}
