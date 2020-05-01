#ifndef MAPPISTONBUTTON_H
#define MAPPISTONBUTTON_H

#include "map.h"
#include "mappiston.h"

namespace Map{
    class MapPiston;

    class MapPistonButton : public MapElement
    {
    private:
        Color _color;

    public:
        MapPistonButton(int, int, Color);
        QString typeName() const override {return "button";}
        Color color() const {return _color;}
        QJsonObject toJsonObject() const override;
    };
}

#endif // MAPPISTONBUTTON_H
