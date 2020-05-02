#include "map.h"
#include "mapmovablecharacter.h"
#include "mapground.h"
#include "mapwall.h"
#include "mappiston.h"
#include "mapfinish.h"

Map::Map::Map(int w, int h) :
    _width(w),
    _height(h)
{
    board = new QSharedPointer<MapElement>*[h];
    for(int i = 0; i < h; ++i)
        board[i] = new QSharedPointer<MapElement>[w];
}

bool Map::Map::canMoveTo(int x, int y) const {
    return board[x][y]->mayBeOccupied() && board[x][y]->occupyingElement().isNull();
}

void Map::Map::placeMovable(QSharedPointer<MapElementMovable> movable) {
    if(canMoveTo(movable->x(), movable->y())){
        board[movable->x()][movable->y()]->_occupyingElement = movable;
    }
}

void Map::Map::moveTo(int oldX, int oldY, int newX, int newY) {
    if(canMoveTo(newX, newY)){
        board[newX][newY]->_occupyingElement.swap(board[oldX][oldY]->_occupyingElement);
    }
}

void Map::Map::update() {
    for(int i = 0; i < _height; ++i)
        for(int j = 0; j < _width; ++j){
             board[i][j]->updateAction();
        }
}

QSharedPointer<Map::MapElementMovable> Map::MapElement::removeOccupyingElement() {
    QSharedPointer<MapElementMovable> _data = _occupyingElement;
    _occupyingElement = QSharedPointer<MapElementMovable>(nullptr);
    return _data;
}

Map::Map::~Map() {
    for(int i = 0; i < _height; ++i)
        delete[] board[i];
    delete[] board;
}

// MAP JSON READING AND WRITING

void Map::Map::fromJsonArray(QJsonArray arr) {
    while(!arr.isEmpty()){
        QJsonObject element = arr.first().toObject();
        arr.pop_front();

        QString typeName = element.value("type").toString();
        int x = element.value("x").toInt();
        int y = element.value("y").toInt();
        if(typeName == "ground")
            board[x][y] = QSharedPointer<MapElement>(new MapGround(this, x, y));

        else if(typeName == "wall")
            board[x][y] = QSharedPointer<MapElement>(new MapWall(this, x, y));
        else if(typeName == "piston"){
            int buttonX = element.value("buttonx").toInt();
            int buttonY = element.value("buttony").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapPiston(this, x, y, buttonX, buttonY));
        }
        else if(typeName == "button"){
            int red = element.value("r").toInt();
            int green = element.value("g").toInt();
            int blue = element.value("b").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapPistonButton(this, x, y, QColor(red, green, blue)));
        }
        else if(typeName == "finish"){
            int id = element.value("playerID").toInt();

            int red = element.value("r").toInt();
            int green = element.value("g").toInt();
            int blue = element.value("b").toInt();
            board[x][y] = QSharedPointer<MapElement>(new MapFinish(this, x, y, id, QColor(red, green, blue)));
        }
        if(element.contains("character")){
            QJsonObject characterObject = element.value("character").toObject();
            int id = characterObject.value("id").toInt();

            int red = characterObject.value("r").toInt();
            int green = characterObject.value("g").toInt();
            int blue = characterObject.value("b").toInt();
            QSharedPointer<MapElementMovable> characterPtr(new MapMovableCharacter(this, x, y, id, QColor(red,green,blue)));
            placeMovable(characterPtr);
        }
    }
}

QSharedPointer<Map::Map> Map::fromJsonObject(QJsonObject mapObject) {
    if(mapObject.value("type") != "map")
        throw new std::exception();

    int width = mapObject.value("width").toInt();
    int height = mapObject.value("height").toInt();
    QSharedPointer<Map> map(new Map(width, height));

    map->fromJsonArray(mapObject.take("elements").toArray());

    return map;
}
