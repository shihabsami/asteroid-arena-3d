#ifndef GAME_ARENA_H
#define GAME_ARENA_H

#include "wall.h"
#include "../utilities/mesh_component.h"
#include "../utilities/lighting_component.h"

#include <memory>

using std::array;
using std::unique_ptr;
using std::make_unique;

enum wall_orientation {
    FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT
};

class game_arena {
public:
    game_arena(double width, double height, double length);

    void init_walls();

    void draw() const;

    const double width;
    const double height;
    const double length;
    array<unique_ptr<wall>, 6> walls;
};

#endif // !GAME_ARENA_H
