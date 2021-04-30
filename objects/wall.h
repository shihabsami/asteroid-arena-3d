#ifndef WALL_H
#define WALL_H

#include "../math/point3d.h"
#include "../main/config.h"

#include <memory>

using std::array;
using std::shared_ptr;
using std::make_shared;

class wall {
public:
    wall(const shared_ptr<array<point3d, 4>>& points, const shared_ptr<color_t>& color);

    void draw() const;

    shared_ptr<array<point3d, 4>> points;
    shared_ptr<color_t> color;
};

#endif // !WALL_H
