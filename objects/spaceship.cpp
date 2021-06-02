#include "spaceship.h"
#include "../main/config.h"
#include "../main/global.h"
#include "../math/game_math.h"

#include <cmath>

spaceship::spaceship(shared_ptr<mesh_t>& body, shared_ptr<mesh_t>& l_wing, shared_ptr<mesh_t>& r_wing,
    shared_ptr<vector3d>& position, shared_ptr<quaternion>& rotation, shared_ptr<vector3d>& scale,
    shared_ptr<vector3d>& direction, double velocity)
    : game_object(body, position, rotation, scale, direction, velocity) {}

void spaceship::init() {
    radius = sqrt(scale->x * scale->x + scale->y * scale->y + scale->z * scale->z);
    t = 0.0;
    is_moving = false;
}

void spaceship::movement() {

}

void spaceship::pitch(bool negative, double amount) {
    double angle = (negative ? -SPACESHIP_STEERING_MODIFIER : SPACESHIP_STEERING_MODIFIER)
        * amount * global::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            global::x_axis->x * sin(to_radians(angle * 0.5)),
            global::x_axis->y * sin(to_radians(angle * 0.5)),
            global::x_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    *direction = global::forward->get_rotated(*rotation);

    global::orientation = rotation;
    global::x_axis->rotate(qr);
    global::y_axis->rotate(qr);
    global::z_axis->rotate(qr);
}

void spaceship::roll(bool negative, double amount) {
    double angle = (negative ? -SPACESHIP_STEERING_MODIFIER : SPACESHIP_STEERING_MODIFIER)
        * amount * global::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            global::z_axis->x * sin(to_radians(angle * 0.5)),
            global::z_axis->y * sin(to_radians(angle * 0.5)),
            global::z_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    *direction = global::forward->get_rotated(*rotation);

    global::x_axis->rotate(qr);
    global::y_axis->rotate(qr);
    global::z_axis->rotate(qr);
}

void spaceship::yaw(bool negative, double amount) {
    double angle = (negative ? -SPACESHIP_STEERING_MODIFIER : SPACESHIP_STEERING_MODIFIER)
        * amount * global::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            global::y_axis->x * sin(to_radians(angle * 0.5)),
            global::y_axis->y * sin(to_radians(angle * 0.5)),
            global::y_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    *direction = global::forward->get_rotated(*rotation);

    global::x_axis->rotate(qr);
    global::y_axis->rotate(qr);
    global::z_axis->rotate(qr);
}

void spaceship::draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(position->x, position->y, position->z);
    glMultMatrixd(rotation->to_matrix());
    glScaled(scale->x, scale->y, scale->z);
    mesh->render();
    glPopMatrix();
}
