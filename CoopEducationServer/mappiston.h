#ifndef MAPPISTON_H
#define MAPPISTON_H

#include "map.h"
#include "mappistonbutton.h"

namespace Map{
    class MapPistonButton;

    class MapPiston : public MapElement
    {
    private:
        int b_x, b_y;
    public:
        MapPiston(int, int, int, int);
        QString typeName() const override {return "piston";}

        QJsonObject toJsonObject() const override;
    };
}

#endif // MAPPISTON_H
