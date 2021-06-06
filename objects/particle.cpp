#include "particle.h"
#include "../main/global.h"
#include "../math/game_math.h"

particle::particle(const shared_ptr<mesh_t>& mesh, GLuint texture_id, const vector3d& position,
    const quaternion& rotation, const vector3d& scale, const vector3d& direction, double velocity,
    double d_angle, double decay_rate, int x_textures, int y_textures)
    : game_object(mesh, texture_id, position, rotation, scale, direction, velocity),
    orientation(), d_angle(d_angle), decay_rate(decay_rate) {

    // generate uv positions for animated texture
    double x_step = 1.0 / x_textures;
    double y_step = 1.0 / y_textures;
    double current_x = 0.0;
    double current_y = 1.0;

    // since the texture has empty space, only the first 6 - 2 = 4 columns are read
    bool is_bullet = decay_rate == 0.0;
    y_textures = is_bullet ? y_textures - 2 : y_textures;

    for (int i = 0; i < y_textures; i++) {
        for (int j = 0; j < x_textures; ++j) {
            texcoords.emplace_back(
                array<point2d, 4>{
                    point2d{ current_x, current_y - y_step },
                    point2d{ current_x + x_step, current_y - y_step },
                    point2d{ current_x + x_step, current_y },
                    point2d{ current_x, current_y }
                }
            );

            current_x += x_step;
        }

        current_x = 0.0;
        current_y -= y_step;
    }

    frame_time = 1.0 / (x_textures * y_textures);
}

void particle::init() {
    radius = sqrt(scale.x / 2 * scale.x / 2 + scale.y / 2 * scale.y / 2 + scale.z / 2 * scale.z / 2);
    t = 1.0;
    texcoord_index = 0;
    l_texcoord_time = 0.0;

    // form triangle mesh into quad for easier animated particles
    quad.at(0) = mesh->faces.at(0).vertices.at(0);
    quad.at(1) = mesh->faces.at(0).vertices.at(1);
    quad.at(2) = mesh->faces.at(0).vertices.at(2);
    quad.at(3) = mesh->faces.at(1).vertices.at(2);
}

void particle::movement() {
    rotation = rotation * quaternion{ d_angle * global::d_time, vector3d::forward() };
    position = position + (direction * velocity * global::d_time);
}

void particle::update() {
    t = t > 0.0 ? t - decay_rate * global::d_time : t;
    t = t < 0.0 ? 0.0 : t;
}

void particle::draw() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(orientation.to_matrix());
    glMultMatrixd(rotation.to_matrix());
    glScaled(scale.x, scale.y, scale.z);

    set_material(mesh->material);
    glBegin(GL_QUADS);
    for (int i = 0; i < quad.size(); ++i) {
        glNormal3d(quad.at(i).normal.x, quad.at(i).normal.y, quad.at(i).normal.z);
        glTexCoord2d(texcoords.at(texcoord_index).at(i).x, texcoords.at(texcoord_index).at(i).y);
        glVertex3d(quad.at(i).position.x, quad.at(i).position.y, quad.at(i).position.z);
    }
    glEnd();

    l_texcoord_time += global::d_time;
    if (l_texcoord_time > frame_time) {
        texcoord_index = (texcoord_index + 1) % (int)texcoords.size();
        l_texcoord_time = 0.0;

        if (texcoord_index == texcoords.size() - 1)
            t = 0.0;
    }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    error_check("particle::draw_particles");
}
