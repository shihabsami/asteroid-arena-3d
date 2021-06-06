#include "spaceship.h"
#include "../main/config.h"
#include "../main/global.h"
#include "../math/game_math.h"
#include "../utilities/input_handler.h"

spaceship::spaceship(
    const shared_ptr<mesh_t>& body, const shared_ptr<mesh_t>& l_wing, const shared_ptr<mesh_t>& r_wing, GLuint texture_id,
    const vector3d& position, const quaternion& rotation, const vector3d& scale,
    const vector3d& direction, double velocity)
    : game_object(body, texture_id, position, rotation, scale, direction, velocity),
    l_wing(l_wing), r_wing(r_wing),
    l_wing_rotation(SPACESHIP_WING_MAX_ROTATION, vector3d::forward()),
    r_wing_rotation(SPACESHIP_WING_MAX_ROTATION, vector3d::forward() * -1.0) {
    point_light = GL_LIGHT1;
}

spaceship::~spaceship() {
    l_wing.reset();
    r_wing.reset();
}

void spaceship::init() {
    radius = sqrt(scale.x / 2 * scale.x / 2 + scale.y / 2 * scale.y / 2 + scale.z / 2 * scale.z / 2);
    t = 0.0;
}

void spaceship::movement() {
    if (input::key_states[W])
        pitch(-1.0);
    if (input::key_states[S])
        pitch(1.0);
    if (input::key_states[D])
        roll(1.0);
    if (input::key_states[A])
        roll(-1.0);
    if (input::key_states[Q])
        yaw(1.0);
    if (input::key_states[E])
        yaw(-1.0);

    if (input::key_states[SPACEBAR]) {
        t = (t < 1.0) ? t + SPACESHIP_SPEED_UP_RATE * global::d_time : t;
        t = t > 1.0 ? 1.0 : t;
        test = true;
    } else {
        t = (t > 0.0) ? t - SPACESHIP_SPEED_UP_RATE * global::d_time : t;
        t = t < 0.0 ? 0.0 : t;
        test = false;
    }

    pitch(input::y_delta);
    yaw(input::x_delta);

    double current_v = lerp(0.0, velocity, t);
    position += direction * current_v * global::d_time;
}

void spaceship::pitch(double amount) {
    // clamp values to prevent weird quaternion glitches
    amount = amount > 1.0 ? 1.0 : amount;
    amount = amount < -1.0 ? -1.0 : amount;
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::right());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::roll(double amount) {
    // clamp values to prevent weird quaternion glitches
    amount = amount > 1.0 ? 1.0 : amount;
    amount = amount < -1.0 ? -1.0 : amount;
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::forward());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::yaw(double amount) {
    // clamp values to prevent weird quaternion glitches
    amount = amount > 1.0 ? 1.0 : amount;
    amount = amount < -1.0 ? -1.0 : amount;
    double angle = SPACESHIP_ROTATION_DELTA * amount * global::d_time;
    rotation = rotation * quaternion(angle, vector3d::up());
    direction = vector3d::forward().get_rotated(rotation);
}

void spaceship::draw() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    vector3d up_offset = position - direction * radius;
    GLfloat light_position[4] {
        (GLfloat) up_offset.x, (GLfloat) up_offset.y, (GLfloat) up_offset.z, 1.0f};
    if (test)
        set_light(point_light, lerp(material::black, material::point_light, t), light_position);

    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.to_matrix());
    glScaled(scale.x, scale.y, scale.z);
    mesh->render();

    glPushMatrix();
    glMultMatrixd(quaternion::slerp(quaternion{}, l_wing_rotation, t).to_matrix());
    l_wing->render();
    glPopMatrix();

    glPushMatrix();
    glMultMatrixd(quaternion::slerp(quaternion{}, r_wing_rotation, t).to_matrix());
    r_wing->render();
    glPopMatrix();

    disable_light(point_light);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    error_check("spaceship::draw_particles");
}
