#ifndef GAME_ARENA_H
#define GAME_ARENA_H

#include "wall.h"

#include <memory>

using std::array;
using std::unique_ptr;
using std::make_unique;

enum wall_orientation {
    UP, DOWN, LEFT, RIGHT, FRONT, BACK
};

class game_arena {
public:
    game_arena(double&& width, double&& height, double&& length);

    void init_walls();

    void draw();

    const double w;
    const double h;
    const double l;
    const array<unique_ptr<wall>, 6> walls;
};

#endif // !GAME_ARENA_H
