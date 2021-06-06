#ifndef LIGHTING_COMPONENT_H
#define LIGHTING_COMPONENT_H

#include "../math/point2d.h"
#include "../math/point3d.h"
#include "../main/graphics.h"

struct color_t {
    GLdouble r, g, b;
};

// color namespace, containing all the colors used
namespace color {
    const color_t white{ 1.0, 1.0, 1.0 };
    const color_t black{ 0.0, 0.0, 0.0 };
    const color_t gray{ 0.25, 0.25, 0.25 };
    const color_t red{ 1.0, 0.0, 0.0 };
    const color_t green{ 0.0, 1.0, 0.0 };
    const color_t blue{ 0.0, 0.0, 1.0 };
    const color_t purple{ 1.0, 0.0, 1.0 };
}

struct material_t {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;
};

// material namespace, containing all the materials used
namespace material {
    const material_t white{
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        0.0 };

    const material_t red{
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.4f, 0.0f, 0.0f, 1.0f },
        0.0 };

    const material_t green{
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.4f, 0.0f, 1.0f },
        0.0 };

    const material_t blue{
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.4f, 1.0f },
        0.0 };

    const material_t light{
        { 0.2f, 0.2f, 0.2f, 1.0f },
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        0.0 };

    const material_t point_light{
        { 0.0f, 0.0f, 0.2f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        1.0 };

    const material_t black{
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        1.0 };


    const material_t wall_white{
        { 1.0f, 1.0f, 1.0f, 0.4f },
        { 1.0f, 1.0f, 1.0f, 0.4f },
        { 1.0f, 1.0f, 1.0f, 0.4f },
        { 0.0f, 0.0f, 0.0f, 0.4f },
        0.0
    };

    const material_t wall_red{
        { 1.0f, 0.0f, 0.0f, 0.8f },
        { 1.0f, 0.0f, 0.0f, 0.8f },
        { 1.0f, 0.0f, 0.0f, 0.8f },
        { 0.0f, 0.0f, 0.0f, 0.8f },
        8.0
    };

    const material_t spaceship_body{
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.8f, 0.0f, 0.4f, 1.0f },
        { 0.8f, 0.0f, 0.4f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        64.0 };

    const material_t spaceship_wing{
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.4f, 0.0f, 0.8f, 1.0f },
        { 0.4f, 0.0f, 0.8f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        64.0 };

    const material_t asteroid{
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.8f, 0.8f, 0.8f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        1.0 };

    const material_t particle{
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        0.0 };
}

static void set_color(color_t color) {
    glColor3d(color.r, color.g, color.b);
}

static void set_light(GLenum light, const material_t& m, const GLfloat* position) {
    glMatrixMode(GL_MODELVIEW);
    glLightfv(light, GL_AMBIENT, m.ambient);
    glLightfv(light, GL_DIFFUSE, m.diffuse);
    glLightfv(light, GL_SPECULAR, m.specular);
    glLightfv(light, GL_POSITION, position);
    glEnable(light);
}

static void disable_light(GLenum light) {
    glMatrixMode(GL_MODELVIEW);
    glDisable(light);
}

static void set_material(const material_t& m) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m.emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m.shininess);

    glColor3d(color::white.r, color::white.g, color::white.b);
}

#endif // !LIGHTING_COMPONENT_H
