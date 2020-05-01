#include "mapfinish.h"

Map::MapFinish::MapFinish(int x, int y, int id, Color color) : MapElement(x, y), _playerID(id), _color(color) {
    this->_mayBeOccupied = true;
}

QJsonObject Map::MapFinish::toJsonObject() const {
    QJsonObject obj = MapElement::toJsonObject();
    obj.insert("playerID", _playerID);
    obj.insert("r", _color.red);
    obj.insert("g", _color.green);
    obj.insert("b", _color.blue);
    return obj;
}
