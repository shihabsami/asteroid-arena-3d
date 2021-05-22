#include "wall.h"

using std::move;

wall::wall(mesh_t mesh, const material_t& material)
    : mesh(move(mesh)), material(material) {}


void wall::draw() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    set_material(material);
    mesh.render();

    glPopMatrix();

    error_check("wall::draw");
}

