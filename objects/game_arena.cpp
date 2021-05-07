#include "game_arena.h"
#include <algorithm>

game_arena::game_arena(double&& width, double&& height, double&& length)
    : w(width), h(height), l(length) {
    g::arena_x_min = -width / 2;
    g::arena_x_max = width / 2;

    g::arena_y_min = -height / 2;
    g::arena_y_max = height / 2;

    g::arena_z_min = length / 2;
    g::arena_z_max = -length / 2;

    init_walls();
}

void game_arena::init_walls() {

    array<shared_ptr<point3d>, 4> points{
        make_shared<point3d>(point3d(-w / 2, h / 2, l / 2)),
        make_shared<point3d>(point3d(-w / 2, h / 2, -l / 2)),
        make_shared<point3d>(point3d(w / 2, h / 2, -l / 2)),
        make_shared<point3d>(point3d(w / 2, h / 2, l / 2)),
    };

    walls[UP] = make_unique<wall>(wall(points, c::white));

    walls[DOWN] = new wall
        (
            { g::arena_x_min, g::arena_y_min },
            { g::arena_x_max, g::arena_y_min }, c::arena_wall
        );
    walls[LEFT] = new wall
        (
            { g::arena_x_min, g::arena_y_min },
            { g::arena_x_min, g::arena_y_max }, c::arena_wall
        );
    walls[RIGHT] = new wall
        (
            { g::arena_x_max, g::arena_y_min },
            { g::arena_x_max, g::arena_y_max }, c::arena_wall
        );
}

void game_arena::draw() {
    for (wall* w : walls) w->draw();
    error_check("game_arena::draw");
}
