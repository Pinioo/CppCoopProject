#include "mapmovablecharacter.h"

Map::MapMovableCharacter::MapMovableCharacter(Map* map, int x, int y, int id, QColor color) : MapElementMovable(map, x, y), _playerID(id), _color(color) {

}

void Map::MapMovableCharacter::move(int x, int y) {
    if(map->canMoveTo(x,y)){
        map->moveTo(_x, _y, x, y);
        _x = x;
        _y = y;
    }
}
