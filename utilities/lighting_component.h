#ifndef LIGHTING_COMPONENT_H
#define LIGHTING_COMPONENT_H

#include "../main/graphics.h"

struct material {
    const GLfloat ambient[4];
    const GLfloat diffuse[4];
    const GLfloat specular[4];
    const GLfloat emission[4];
    const GLfloat shininess;
};

static void gl_light(GLenum light, const material& m, const point3d& position) {
    glLightfv(light, GL_AMBIENT, m.ambient);
    glLightfv(light, GL_DIFFUSE, m.diffuse);
    glLightfv(light, GL_SPECULAR, m.specular);
    glLightfv(light, GL_EMISSION, m.emission);
    glLightf(light, GL_SHININESS, m.shininess);

    const GLfloat gl_position[3]{ static_cast<GLfloat>(position.x), static_cast<GLfloat>(position.y),
        static_cast<GLfloat>(position.z) };
    glLightfv(light, GL_POSITION, gl_position);
}

static void gl_material(const material& m) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m.emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m.shininess);
}

#endif // !LIGHTING_COMPONENT_H
