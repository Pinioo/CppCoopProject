#ifndef MAP_H
#define MAP_H

#include <QSharedPointer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

// Map namespace contains Map class and its virtual elements

namespace Map {
    class Map;
    class MapElement;
    class MapElementMovable;

    QSharedPointer<Map> fromJsonObject(QJsonObject);

    struct Color{
        int red, green, blue;
    };

    class Map {
    private:
        Color player1Color = {255,0,0};
        Color player2Color = {0,0,255};
        int _width, _height;
        QSharedPointer<MapElement>** board;

    public:
        Map(int w, int h);
        void makeDefault();
        void loadMap1();
        void loadMap2();
        void loadMap3();
        QSharedPointer<MapElement> getElem(int x, int y) {return board[x][y];}

        int width() const {return _width;};
        int height() const {return _height;};

        bool canMoveTo(int, int) const;
        void placeMovable(QSharedPointer<MapElementMovable> movable);
        void moveTo(int oldX, int oldY, int newX, int newY);
        void update();


        void fromJsonArray(QJsonArray);
        QJsonObject toJsonObject() const;

        void fromFile(const QString filename);
        void toFile(const QString filename) const;
    };

    class MapElement
    {
    protected:
        int _x, _y;
        bool _mayBeOccupied{false};
        QSharedPointer<MapElementMovable> _occupyingElement{QSharedPointer<MapElementMovable>(nullptr)};

    public:
        MapElement(int x, int y) : _x(x), _y(y){}

        int x() const {return _x;}
        int y() const {return _y;}

        virtual QString typeName() const = 0;

        bool mayBeOccupied() const {return _mayBeOccupied;}
        QSharedPointer<MapElementMovable> occupyingElement() {return _occupyingElement;}

        QSharedPointer<MapElementMovable> removeOccupyingElement();

        virtual ~MapElement() = default;
        virtual QJsonObject toJsonObject() const;

        friend void Map::placeMovable(QSharedPointer<MapElementMovable> movable);
    };

    class MapElementMovable : public MapElement
    {
    public:
        MapElementMovable(int x, int y) : MapElement(x,y){};
        virtual ~MapElementMovable() = default;

    };
}

#endif // MAP_H
