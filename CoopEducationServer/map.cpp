#include "map.h"
#include "mapmovablecharacter.h"
#include "mapground.h"
#include "mapwall.h"
#include "mappiston.h"
#include "mapfinish.h"

#include <QFile>
#include <QTextStream>
#include <QVector>

Map::Map::Map(int w, int h) :
    _width(w),
    _height(h)
{
    board = new QSharedPointer<MapElement>*[h];
    for(int i = 0; i < h; ++i)
        board[i] = new QSharedPointer<MapElement>[w];
}

bool Map::Map::canMoveTo(int x, int y) const
{
    return board[x][y]->mayBeOccupied() && board[x][y]->occupyingElement().isNull();
}

void Map::Map::placeMovable(QSharedPointer<MapElementMovable> movable) {
    if(canMoveTo(movable->x(), movable->y())){
        board[movable->x()][movable->y()]->_occupyingElement = movable;
    }
}

QSharedPointer<Map::MapElementMovable> Map::MapElement::removeOccupyingElement() {
    QSharedPointer<MapElementMovable> _data = _occupyingElement;
    _occupyingElement = QSharedPointer<MapElementMovable>(nullptr);
    return _data;
}

// MAP JSON HANDLING

void Map::Map::fromJsonArray(QJsonArray arr) {
    while(!arr.isEmpty()){
        QJsonObject element = arr.first().toObject();
        arr.pop_front();

        QString typeName = element.value("type").toString();
        int x = element.value("x").toInt();
        int y = element.value("y").toInt();
        if(typeName == "ground")
            board[x][y] = QSharedPointer<MapElement>(new MapGround(x, y));

        else if(typeName == "wall")
            board[x][y] = QSharedPointer<MapElement>(new MapWall(x, y));
        else if(typeName == "piston"){
            int buttonX = element.value("buttonx").toInt();
            int buttonY = element.value("buttony").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapPiston(x, y, buttonX, buttonY));
        }
        else if(typeName == "button"){
            int red = element.value("r").toInt();
            int green = element.value("g").toInt();
            int blue = element.value("b").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapPistonButton(x, y, {red, green, blue}));
        }
        else if(typeName == "finish"){
            int id = element.value("playerID").toInt();

            int red = element.value("r").toInt();
            int green = element.value("g").toInt();
            int blue = element.value("b").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapFinish(x, y, id, {red, green, blue}));
        }
        if(element.contains("character")){
            QJsonObject characterObject = element.value("character").toObject();
            int id = characterObject.value("id").toInt();

            int red = characterObject.value("r").toInt();
            int green = characterObject.value("g").toInt();
            int blue = characterObject.value("b").toInt();
            QSharedPointer<MapElementMovable> characterPtr(new MapMovableCharacter(x, y, id, {red,green,blue}));
            placeMovable(characterPtr);
        }
    }
}

QSharedPointer<Map::Map> Map::fromJsonObject(QJsonObject mapObject)
{
    if(mapObject.value("type") != "map")
        throw new std::exception();

    int width = mapObject.value("width").toInt();
    int height = mapObject.value("height").toInt();
    QSharedPointer<Map> map(new Map(width, height));

    map->fromJsonArray(mapObject.take("elements").toArray());

    return map;
}

QJsonObject Map::Map::toJsonObject() const
{
    QJsonObject mapObject;
    mapObject.insert("type", "map");
    mapObject.insert("width", _width);
    mapObject.insert("height", _height);

    QJsonArray mapElementsArray;
    for(int y = 0; y < _height; ++y)
        for(int x = 0; x < _width; ++x){
            mapElementsArray.append(board[x][y]->toJsonObject());
        }
    mapObject.insert("elements", mapElementsArray);
    return mapObject;
}

void Map::Map::fromFile(const QString filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonObject obj = QJsonDocument().fromJson(file.readAll()).object();
        if(obj.value("type").toString() == "map"){
            _width = obj.value("width").toInt();
            _height = obj.value("height").toInt();
            this->fromJsonArray(obj.value("elements").toArray());
        }
        file.close();
    }
    else {
        throw std::invalid_argument("File doesn't exist");
    }
}

void Map::Map::toFile(const QString filename) const{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << QJsonDocument(this->toJsonObject()).toJson();
        file.close();
    }
}

QJsonObject Map::MapElement::toJsonObject() const
{
    QJsonObject elementObject;
    elementObject.insert("x", _x);
    elementObject.insert("y", _y);
    elementObject.insert("type", this->typeName());
    if(_mayBeOccupied && _occupyingElement != nullptr){
        elementObject.insert("character", _occupyingElement->toJsonObject());
    }
    return elementObject;
}
