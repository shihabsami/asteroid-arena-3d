#ifndef PARTICLE_H
#define PARTICLE_H

#include "game_object.h"
#include "../main/config.h"
#include "../math//point2d.h"

#include <vector>

using std::vector;

class particle : public game_object {
public:
    particle(const shared_ptr<mesh_t>& mesh, GLuint texture_id,
        const vector3d& position, const quaternion& rotation, const vector3d& scale,
        const vector3d& direction, double velocity, double d_angle, double decay_rate, int x_textures, int y_textures);

    void init() override;

    void update();

    void movement() override;

    void draw() override;

    double d_angle{};
    vector3d rotation_axis;
    quaternion orientation;
    array<vertex_t, 4> quad;
    vector<array<point2d, 4>> texcoords;
    int texcoord_index{};
    double frame_time;
    double l_texcoord_time{};
    double t{};
    double decay_rate{};
};

#endif // !PARTICLE_H
