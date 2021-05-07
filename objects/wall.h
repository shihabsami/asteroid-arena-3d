#ifndef WALL_H
#define WALL_H

#include "../math/point3d.h"
#include "../main/config.h"


class wall {
public:
    wall(const point3d* points, const color_t* color);

    void draw() const;

    const point3d* points;
    const color_t* color;
};

#endif // !WALL_H
