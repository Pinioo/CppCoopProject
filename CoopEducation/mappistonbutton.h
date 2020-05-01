#ifndef MAPPISTONBUTTON_H
#define MAPPISTONBUTTON_H

#include "map.h"

namespace Map{
    class MapPistonButton : public MapElement
    {
    private:
        QColor _color;

    public:
        MapPistonButton(Map*, int, int, QColor);
        QColor color() const{return _color;}
    };
}

#endif // MAPPISTONBUTTON_H
