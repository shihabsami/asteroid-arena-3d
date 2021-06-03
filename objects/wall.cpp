#include "wall.h"

using std::move;

wall::wall(const vector3d& position, vector<line_t>& lines, const material_t& material)
    : position(position), lines(move(lines)), material(material) {}

void wall::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    set_material(material);
    glLineWidth(ARENA_WALL_WIDTH);
    glBegin(GL_LINES);
    for (const auto& line : lines) {
        glVertex3d(line.start.x, line.start.y, line.start.z);
        glVertex3d(line.end.x, line.end.y, line.end.z);
    }
    glEnd();

    glPopMatrix();
    error_check("wall::display");
}
