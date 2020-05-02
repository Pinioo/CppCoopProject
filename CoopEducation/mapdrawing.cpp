#include "mapmovablecharacter.h"
#include "mapdrawing.h"
#include "mapground.h"
#include "mapwall.h"
#include "mappiston.h"
#include "mapfinish.h"

MapDrawing::MapDrawing(QSharedPointer<Map::Map> map) :
    map(map)
{
    loaded = QImage(QString("resources/textures/ground.jpg"), "jpg");
    image = loaded.scaled(800,600);

    paintOnImage = new QPainter();
}

void MapDrawing::resetMap(){
    image = loaded.scaled(800,600);
}

void MapDrawing::drawMap()
{
    if(map != nullptr)
    {
        image = loaded.scaled(800,600);
        paintOnImage->begin(&image);
        for(int i = 0; i < map->height(); ++i)
            for(int j = 0; j < map->width(); ++j){
                drawElement(map->getElem(i,j));
            }
        paintOnImage->end();
    }
}

void MapDrawing::setMap(QSharedPointer<Map::Map> map)
{
    this->map.reset();
    this->map = map;
}

MapDrawing::~MapDrawing()
{
    paintOnImage->end();
    delete paintOnImage;
}

// FUNCTION TO DRAW EVERY ELEMENT

void MapDrawing::drawElement(QSharedPointer<Map::MapElement> el)
{
    paintOnImage->setRenderHint(QPainter::Antialiasing);
    int rectWidth = image.width() / map->width();
    int rectHeight = image.height() / map->height();

    int rectX = el->x()*rectWidth;
    int rectY = el->y()*rectHeight;

    QBrush brush;

    paintOnImage->setPen(groundBorderColor);
    paintOnImage->drawRect(rectX, rectY, rectWidth, rectHeight);

    if(instanceof<Map::MapGround>(el.data())){
        // Ground is already painted
    }
    else if(instanceof<Map::MapWall>(el.data())) {
        // Fill side of the wall
        QPainterPath sidePath;
        sidePath.addRoundedRect(QRectF(rectX, rectY-rectHeight/3, rectWidth, rectHeight+rectHeight/3), 10, 10);
        paintOnImage->fillPath(sidePath, wallSideColor);

        // Fill rounded rectangle on top
        QPainterPath topPath;
        topPath.addRoundedRect(QRectF(rectX + 1.5, rectY-rectHeight/3 + 1.5, rectWidth - 3, rectHeight - 3), 10, 10);
        paintOnImage->fillPath(topPath, wallTopColor);
    }
    else if(instanceof<Map::MapPiston>(el.data())) {
        QColor pistonColor = el.dynamicCast<Map::MapPiston>()->color();
        if(el->mayBeOccupied()){
            // Fill side
            QPainterPath sidePath;
            sidePath.addRoundedRect(QRectF(rectX, rectY, rectWidth, rectHeight), 10, 10);
            paintOnImage->fillPath(sidePath, pistonColor);

            // Fill rounded rectangle
            QPainterPath topPath;
            topPath.addRoundedRect(QRectF(rectX + 1.5, rectY + 1.5, rectWidth - 3, rectHeight - 3), 10, 10);
            paintOnImage->fillPath(topPath, wallTopColor);
        }
        else {
            // Fill side of the wall
            QPainterPath sidePath;
            sidePath.addRoundedRect(QRectF(rectX, rectY-rectHeight/3, rectWidth, rectHeight+rectHeight/3), 10, 10);
            paintOnImage->fillPath(sidePath, pistonColor);

            // Fill rounded rectangle on top
            QPainterPath topPath;
            topPath.addRoundedRect(QRectF(rectX + 1.5, rectY-rectHeight/3 + 1.5, rectWidth - 3, rectHeight - 3), 10, 10);
            paintOnImage->fillPath(topPath, wallTopColor);
        }
    }
    else if(instanceof<Map::MapPistonButton>(el.data())) {
        paintOnImage->fillRect(rectX, rectY, rectWidth, rectHeight, el.dynamicCast<Map::MapPistonButton>()->color());

    }
    else if(instanceof<Map::MapFinish>(el.data())) {
        paintOnImage->setPen(QPen(el.dynamicCast<Map::MapFinish>()->color(), 2));
        paintOnImage->drawRect(rectX+2, rectY+2, rectWidth-4, rectHeight-4);
    }

    if(!el->occupyingElement().isNull()){
        if(instanceof<Map::MapMovableCharacter>(el->occupyingElement().data())){
            QPainterPath character;
            int characterWidth = rectWidth/2;
            int characterHeight = rectHeight/2;
            character.addRoundedRect(QRectF(rectX + rectWidth/2 - characterWidth/2, rectY + rectHeight/2 - characterHeight/2, characterWidth, characterHeight), 2, 2);
            paintOnImage->fillPath(character, el->occupyingElement().dynamicCast<Map::MapMovableCharacter>()->color());

            QPainterPath directionPath;
            int smallRectWidth = characterWidth/3;
            int smallRectHeight = characterHeight/3;
            switch(el->occupyingElement().dynamicCast<Map::MapMovableCharacter>()->direction()){
                case Map::MapMovableCharacter::Direction::NORTH:
                    directionPath.addRect(QRectF(rectX + rectWidth/2 - smallRectWidth/2, rectY + rectHeight/2 - characterHeight/2, smallRectWidth, smallRectHeight)); break;
                case Map::MapMovableCharacter::Direction::SOUTH:
                    directionPath.addRect(QRectF(rectX + rectWidth/2 - smallRectWidth/2, rectY + rectHeight/2 + characterHeight/2 - smallRectHeight, smallRectWidth, smallRectHeight)); break;
                case Map::MapMovableCharacter::Direction::WEST:
                    directionPath.addRect(QRectF(rectX + rectWidth/2 - characterWidth/2, rectY + rectHeight/2 - smallRectHeight/2, smallRectWidth, smallRectHeight)); break;
                case Map::MapMovableCharacter::Direction::EAST:
                    directionPath.addRect(QRectF(rectX + rectWidth/2 + characterWidth/2 - smallRectWidth, rectY + rectHeight/2 - smallRectHeight/2, smallRectWidth, smallRectHeight)); break;
            }
            paintOnImage->fillPath(directionPath, QColor(255,255,255));
        }
    }
}
