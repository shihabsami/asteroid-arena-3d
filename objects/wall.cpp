#include "wall.h"

wall::wall(const shared_ptr<array<point3d, 4>>& points, const shared_ptr<color_t>& color)
: points(points), color(color) {}

void wall::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glColor3d(color->r, color->g, color->b);
    glBegin(GL_LINE_LOOP);
    for (const auto& point : *points) glVertex3d(point.x, point.y, point.z);
    glEnd();

    glPopMatrix();
    error_check("wall::draw");
}




