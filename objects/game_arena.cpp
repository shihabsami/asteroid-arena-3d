#include "game_arena.h"
#include <algorithm>

game_arena::game_arena(double width, double height, double length)
    : width(width), height(height), length(length) {
    g::arena_x_min = -width / 2;
    g::arena_x_max = width / 2;
    g::arena_y_min = -height / 2;
    g::arena_y_max = height / 2;
    g::arena_z_min = length / 2;
    g::arena_z_max = -length / 2;

    init_walls();
}

void game_arena::init_walls() {
    array<point3d, 4> points{
        point3d(-width / 2, height / 2, length / 2),
        point3d(-width / 2, height / 2, -length / 2),
        point3d(width / 2, height / 2, -length / 2),
        point3d(width / 2, height / 2, length / 2),
    };

    shared_ptr<array<point3d, 4>> v_up_wall = make_shared<array<point3d, 4>>(points);
    shared_ptr<color_t> c_wall = make_shared<color_t>(c::white);
    walls[UP] = make_unique<wall>(wall(v_up_wall, c_wall));
}

void game_arena::draw() const {
    for (const auto& w : walls) w->draw();
    error_check("game_arena::draw");
}
