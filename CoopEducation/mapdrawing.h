#ifndef MAPDRAWING_H
#define MAPDRAWING_H

#include "map.h"
#include "q_includes.h"
#include "instanceof.h"

class MapDrawing
{
public:
    MapDrawing(QSharedPointer<Map::Map> map);

    void drawMap();
    void setMap(QSharedPointer<Map::Map>);
    void resetMap();
    QImage image;

    ~MapDrawing();

private:
    void drawElement(QSharedPointer<Map::MapElement> el);

    QImage loaded;
    QPainter* paintOnImage;

    QSharedPointer<Map::Map> map;

    QColor wallTopColor = QColor(255, 87, 34);
    QColor wallSideColor = QColor(221, 44, 0);
    QColor groundBorderColor = QColor(36,36,36);
};

#endif // MAPDRAWING_H
