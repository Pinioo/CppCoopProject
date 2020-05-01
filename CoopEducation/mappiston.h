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
        MapPiston(Map*, int, int, int, int);
        void updateAction() override;
        QColor color() const {return map->getElem(b_x, b_y).dynamicCast<MapPistonButton>()->color();}
    };
}

#endif // MAPPISTON_H
