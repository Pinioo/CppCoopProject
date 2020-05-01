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

void Map::Map::loadMap1()
{
    for(int i = 0; i < _width; ++i)
        for(int j = 0; j < _height; ++j){
            if(i < 5 || j < 6 || i > _width - 6 || j > _height - 6)
                board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
            else
                board[i][j] = QSharedPointer<MapElement>(new MapGround(i, j));
        }

    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(5, 6, 0, player1Color)));
    board[5][9] = QSharedPointer<MapElement>(new MapFinish(5, 9, 0, player1Color));

    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(5, 9, 1, player2Color)));
    board[9][9] = QSharedPointer<MapElement>(new MapFinish(9, 9, 1, player2Color));
}

void Map::Map::loadMap2() {
    for(int i = 0; i < _width; ++i)
        for(int j = 0; j < _height; ++j){
            if(j == 6 && i == 6)
                board[i][j] = QSharedPointer<MapElement>(new MapPistonButton(i, j, {255,255,0}));
            else if(i == 7){
                if(j == 5)
                    board[i][j] = QSharedPointer<MapElement>(new MapPiston(i, j, 6, 6));
                else
                    board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
            }
            else if(i < 3 || j < 4 || i > _height - 4 || j > _width - 4)
                board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
            else
                board[i][j] = QSharedPointer<MapElement>(new MapGround(i, j));
        }
    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(5, 8, 0, player1Color)));
    board[8][8] = QSharedPointer<MapElement>(new MapFinish(8, 8, 0, player1Color));

    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(9, 8, 1, player2Color)));
    board[5][4] = QSharedPointer<MapElement>(new MapFinish(5, 4, 1, player2Color));
}

void Map::Map::loadMap3() {
    Color pistonColors[] = {{255,255,0}, {0,255,255}, {255,0,255}, {0,255,0}, {255,255,255}, {0,0,0}};
    int buttonsCoords[][2] = {{6,7}, {7,7}, {8,7}, {9,7}, {10,7}, {11,7}};
    QVector<int> coordsIndexes;
    for(int i = 0; i < 6; ++i)
        coordsIndexes.append(i);

    srand(time(NULL));
    for(int i = 0; i < 6; ++i){
        int tmpInd = rand() % (6-i);
        int tmpIndCoord = coordsIndexes[tmpInd];
        coordsIndexes.remove(tmpInd);
        coordsIndexes.append(tmpIndCoord);
    }

    for(int i = 0; i < 6; ++i){
        int x = buttonsCoords[i][0];
        int y = buttonsCoords[i][1];
        board[x][y].reset();
        board[x][y] = QSharedPointer<MapElement>(new MapPistonButton(x, y, pistonColors[i]));
    }
    for(int i = 0; i < _width; ++i)
        for(int j = 0; j < _height; ++j){
            if(i < 1 || j < 1 || i > _height - 2 || j > _width - 2)
                board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
            else if(i == 4){
                board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
            }
            else if(i < 4){
                if(j % 2 == 0){
                    if(i == 2)
                        board[i][j] = QSharedPointer<MapElement>(new MapPiston(i, j, buttonsCoords[coordsIndexes[j/2 - 1]][0], buttonsCoords[coordsIndexes[j/2 - 1]][1]));
                    else
                        board[i][j] = QSharedPointer<MapElement>(new MapWall(i, j));
                }
                else
                    board[i][j] = QSharedPointer<MapElement>(new MapGround(i, j));
            }
            else if(board[i][j] == nullptr)
                board[i][j] = QSharedPointer<MapElement>(new MapGround(i, j));
        }
    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(2, 13, 0, player1Color)));
    board[2][1] = QSharedPointer<MapElement>(new MapFinish(2, 1, 0, player1Color));

    placeMovable(QSharedPointer<MapElementMovable>(new MapMovableCharacter(9, 9, 1, player2Color)));
    board[12][7] = QSharedPointer<MapElement>(new MapFinish(12, 7, 1, player2Color));
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

QSharedPointer<Map::MapElementMovable> Map::MapElement::removeOccupyingElement() {
    QSharedPointer<MapElementMovable> _data = _occupyingElement;
    _occupyingElement = QSharedPointer<MapElementMovable>(nullptr);
    return _data;
}
