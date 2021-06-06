#include "game_arena.h"
#include "../main/config.h"

game_arena::game_arena(double width, double height, double length)
    : width(2 * width), height(2 * height), length(2 * length) {
    init_walls();
}

void game_arena::init_walls() {
    double x_min = -width / 2;
    double x_max = width / 2;
    double y_min = -height / 2;
    double y_max = height / 2;
    double z_min = length / 2;
    double z_max = -length / 2;

    double x_step = width / ARENA_WALL_DIVISIONS;
    double y_step = height / ARENA_WALL_DIVISIONS;
    double z_step = length / ARENA_WALL_DIVISIONS;
    double x_start, y_start, z_start;

    vector<line_t> pz_lines;
    vector<line_t> nz_lines;
    x_start = x_min;
    y_start = y_min;
    for (size_t i = 0; i < ARENA_WALL_DIVISIONS - 1; ++i) {
        x_start += x_step;
        y_start += y_step;

        line_t pz_horizontal(
            point3d{ x_start, y_max, z_min }, point3d{ x_start, y_min, z_min });
        line_t pz_vertical(
            point3d{ x_max, y_start, z_min }, point3d{ x_min, y_start, z_min });
        pz_lines.emplace_back(pz_horizontal);
        pz_lines.emplace_back(pz_vertical);

        line_t nz_horizontal(
            point3d{ x_start, y_max, z_max }, point3d{ x_start, y_min, z_max });
        line_t nz_vertical(
            point3d{ x_max, y_start, z_max }, point3d{ x_min, y_start, z_max });
        nz_lines.emplace_back(nz_horizontal);
        nz_lines.emplace_back(nz_vertical);
    }

    walls[FRONT] = make_unique<wall>(
        vector3d(0.0, 0.0, z_max), nz_lines);
    walls[BACK] = make_unique<wall>(
        vector3d(0.0, 0.0, z_min), pz_lines);

    vector<line_t> py_lines;
    vector<line_t> ny_lines;
    x_start = x_min;
    z_start = z_max;
    for (size_t i = 0; i < ARENA_WALL_DIVISIONS - 1; ++i) {
        x_start += x_step;
        z_start += z_step;

        line_t py_horizontal(
            point3d{ x_start, y_max, z_min }, point3d{ x_start, y_max, z_max });
        line_t py_vertical(
            point3d{ x_min, y_max, z_start }, point3d{ x_max, y_max, z_start });
        py_lines.emplace_back(py_horizontal);
        py_lines.emplace_back(py_vertical);

        line_t ny_horizontal(
            point3d{ x_start, y_min, z_min }, point3d{ x_start, y_min, z_max });
        line_t ny_vertical(
            point3d{ x_min, y_min, z_start }, point3d{ x_max, y_min, z_start });
        ny_lines.emplace_back(ny_horizontal);
        ny_lines.emplace_back(ny_vertical);
    }

    walls[TOP] = make_unique<wall>(
        vector3d(0.0, y_max, 0.0), py_lines);
    walls[BOTTOM] = make_unique<wall>(
        vector3d(0.0, y_min, 0.0), ny_lines);

    vector<line_t> px_lines;
    vector<line_t> nx_lines;
    y_start = y_min;
    z_start = z_max;
    for (size_t i = 0; i < ARENA_WALL_DIVISIONS - 1; ++i) {
        y_start += y_step;
        z_start += z_step;

        line_t px_horizontal(
            point3d{ x_max, y_start, z_min }, point3d{ x_max, y_start, z_max });
        line_t px_vertical(
            point3d{ x_max, y_min, z_start }, point3d{ x_max, y_max, z_start });
        px_lines.emplace_back(px_horizontal);
        px_lines.emplace_back(px_vertical);

        line_t nx_horizontal(
            point3d{ x_min, y_start, z_min }, point3d{ x_min, y_start, z_max });
        line_t nx_vertical(
            point3d{ x_min, y_min, z_start }, point3d{ x_min, y_max, z_start });
        nx_lines.emplace_back(nx_horizontal);
        nx_lines.emplace_back(nx_vertical);
    }

    walls[LEFT] = make_unique<wall>(
        vector3d(x_min, 0.0, 0.0), nx_lines);
    walls[RIGHT] = make_unique<wall>(
        vector3d(x_max, 0.0, 0.0), px_lines);
}

void game_arena::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    set_material(material::wall_white);
    glutWireCube(width);

    for (const auto& wall : walls) {
        if (wall != nullptr) {
            wall->update();
            wall->draw();
        }
    }

    glPopMatrix();

    error_check("game_arena::draw_particles");
}
