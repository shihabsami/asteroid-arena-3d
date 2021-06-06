#include "wall.h"
#include "../main/global.h"
#include "../math/game_math.h"

using std::move;

wall::wall(const vector3d& position, vector<line_t>& lines)
    : position(position), lines(move(lines)),
    old_material(material::wall_white), new_material(material::wall_white) {}

void wall::current_material(const material_t& m) {
    if (m.shininess != new_material.shininess) {
        t = 0.0;
        old_material = new_material;
        new_material = m;
    }
}

void wall::update() {
    t = t < 1.0 ? t + ARENA_WALL_LERP_FACTOR * global::d_time : t;
    t = t > 1.0 ? 1.0 : t;
}

void wall::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    set_material(lerp(old_material, new_material, t));
    glLineWidth(ARENA_WALL_WIDTH);
    glBegin(GL_LINES);
    for (const auto& line : lines) {
        glVertex3d(line.start.x, line.start.y, line.start.z);
        glVertex3d(line.end.x, line.end.y, line.end.z);
    }
    glEnd();

    glPopMatrix();

    error_check("wall::draw");
}
