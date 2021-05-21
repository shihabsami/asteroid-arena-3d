#ifndef POSITION_H
#define POSITION_H

#include "vector3d.h"
#include "component.h"

struct position : public component {
    position(vector3d v) : v(v) {}

    vector3d v;
};

#endif // !POSITION_H
