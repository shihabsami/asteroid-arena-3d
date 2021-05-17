#ifndef POSITION_H
#define POSITION_H

#include "vector3d.h"
#include "component.h"

struct position : com{
    position(vector3d v) : v(v) {}

    vector3d v;
};

#endif // !POSITION_H
