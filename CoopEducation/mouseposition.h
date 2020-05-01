#ifndef MOUSEPOSITION_H
#define MOUSEPOSITION_H

#include "q_includes.h"

class MousePosition
{
private:
    int _x{0}, _y{0};

public:
    MousePosition() = default;

    int x() const {return _x;}
    int y() const {return _y;}

    void setX(int x){_x = x;}
    void setY(int y){_y = y;}

    ~MousePosition() = default;
};

#endif // MOUSEPOSITION_H
