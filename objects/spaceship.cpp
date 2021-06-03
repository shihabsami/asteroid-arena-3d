#include "spaceship.h"
#include "../main/config.h"
#include "../main/global.h"
#include "../utilities/input_handler.h"

#include <cmath>

spaceship::spaceship(
    const shared_ptr<mesh_t>& body, const shared_ptr<mesh_t>& l_wing, const shared_ptr<mesh_t>& r_wing,
    const vector3d& position, const quaternion& rotation, const vector3d& scale,
    const vector3d& direction, double velocity)
    : game_object(body, position, rotation, scale, direction, velocity), l_wing(l_wing), r_wing(r_wing) {}

spaceship::~spaceship() {
    l_wing.reset();
    r_wing.reset();
}

void spaceship::init() {
    radius = sqrt(scale.x * scale.x + scale.y * scale.y + scale.z * scale.z);
    t = 0.0;
    is_moving = false;
}

void spaceship::movement() {
    if (input::key_states[Q])
        yaw(1.0);
    if (input::key_states[E])
        yaw(-1.0);
    if (input::key_states[W])
        pitch(-1.0);
    if (input::key_states[S])
        pitch(1.0);
    if (input::key_states[D])
        roll(1.0);
    if (input::key_states[A])
        roll(-1.0);
    if (input::key_states[SPACEBAR])
        position += direction * velocity * global::d_time;

    if (input::key_states[X]) {
        pitch(input::y_delta);
        yaw(input::x_delta);
    }
}

void spaceship::pitch(double amount) {
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::right());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::roll(double amount) {
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::forward());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::yaw(double amount) {
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::up());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.to_matrix());
    glScaled(scale.x, scale.y, scale.z);
    mesh->render();
    l_wing->render();
    r_wing->render();
    glPopMatrix();
}
