#include "game_camera.h"
#include "config.h"

game_camera::game_camera(const vector3d& position, const quaternion& orientation)
    : position(position), target_position(position), orientation(orientation), target_orientation(orientation) {}

void game_camera::orient() const {
    glMultMatrixd(orientation.get_inverse().to_matrix());
}

void game_camera::translate() const {
    glTranslated(-position.x, -position.y, -position.z);
}

void game_camera::move(const vector3d& v) {
    t = 0.0;
    target_position = v;
}

void game_camera::rotate(const quaternion& q) {
    t = 0.0;
    target_orientation = q;
}

void game_camera::update() {
    t = t < 1.0 ? t + CAMERA_LAG * g::d_time : t;
    t = t > 1.0 ? 1.0 : t;
    position = vector3d::lerp(position, target_position, t);
    orientation = quaternion::slerp(orientation, target_orientation, t);
}
