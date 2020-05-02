#ifndef MAP_H
#define MAP_H

#include "q_includes.h"
#include "instanceof.h"

// Map namespace contains Map class and its virtual elements

namespace Map {
    class Map;
    class MapElement;
    class MapElementMovable;

    QSharedPointer<Map> fromJsonObject(QJsonObject);

    class Map {
    private:
        int _width, _height;
        QSharedPointer<MapElement>** board;

    public:
        Map(int w, int h);
        void makeDefault();
        QSharedPointer<MapElement> getElem(int x, int y) {return board[x][y];}

        int width() const {return _width;};
        int height() const {return _height;};

        bool canMoveTo(int, int) const;
        void placeMovable(QSharedPointer<MapElementMovable> movable);
        void moveTo(int oldX, int oldY, int newX, int newY);
        void update();

        void fromJsonArray(QJsonArray);

        ~Map();
    };

    class MapElement
    {
    protected:
        int _x, _y;
        bool _mayBeOccupied{false};
        Map* map;
        QSharedPointer<MapElementMovable> _occupyingElement{QSharedPointer<MapElementMovable>(nullptr)};

    public:
        MapElement(Map* map, int x, int y) : _x(x), _y(y), map(map){}

        int x() const {return _x;}
        int y() const {return _y;}

        virtual void updateAction(){};

        bool mayBeOccupied() const {return _mayBeOccupied;}
        QSharedPointer<MapElementMovable> occupyingElement() {return _occupyingElement;}

        QSharedPointer<MapElementMovable> removeOccupyingElement();

        virtual ~MapElement() = default;

        friend void Map::moveTo(int oldX, int oldY, int newX, int newY);
        friend void Map::placeMovable(QSharedPointer<MapElementMovable> movable);
    };

    class MapElementMovable : public MapElement
    {
    public:
        MapElementMovable(Map* map, int x, int y) : MapElement(map, x,y){};
        virtual void move(int, int) = 0;
        virtual ~MapElementMovable() = default;

    };
}

#endif // MAP_H
