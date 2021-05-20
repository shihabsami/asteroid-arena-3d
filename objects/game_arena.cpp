#include "game_arena.h"
#include "../utilities/object_loader.h"

game_arena::game_arena(double width, double height, double length)
    : width(width), height(height), length(length) {
    init_walls();
}

void game_arena::init_walls() {
    // front
    shared_ptr<mesh_t> mesh = load_obj("models/cube.obj");



}

void game_arena::draw() {
    for (const auto& w : walls)
        w->draw();

    error_check("game_arena::draw");
}
