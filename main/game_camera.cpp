#include "game_camera.h"
#include "graphics.h"
#include "config.h"
#include "global.h"
#include "../utilities/input_handler.h"

game_camera::game_camera(const vector3d& position, const quaternion& orientation)
    : position(position), target_position(position), orientation(orientation), target_orientation(orientation) {}

void game_camera::orient() const {
    glMultMatrixd(orientation.get_inverse().to_matrix());
}

void game_camera::translate() const {
    glTranslated(-position.x, -position.y, -position.z);
}

void game_camera::move(const vector3d& v) {
    vector3d back_offset =
        v - vector3d::forward().get_rotated(target_orientation) * CAMERA_Z_OFFSET * zoom_factor;
    vector3d up_offset =
        vector3d::up().get_rotated(target_orientation) * CAMERA_Y_OFFSET * zoom_factor;

    t = 0.0;
    target_position = back_offset + up_offset;

    if (input::mouse_states[WHEEL_UP]) {
        zoom_factor -= 0.05;
        zoom_factor = zoom_factor < 0.1 ? 0.1 : zoom_factor;
        input::mouse_states[WHEEL_UP] = false;
    } else if (input::mouse_states[WHEEL_DOWN]) {
        zoom_factor += 0.05;
        zoom_factor = zoom_factor > 2.0 ? 2.0 : zoom_factor;
        input::mouse_states[WHEEL_DOWN] = false;
    }
}

void game_camera::rotate(const quaternion& q) {
    t = 0.0;
    if (input::key_states[Z]) {
        target_orientation = quaternion{q * quaternion{90.0, vector3d::up()}}.get_normalised();
        orientation = target_orientation;
        is_switched = true;
    } else if (input::key_states[C]) {
        target_orientation = quaternion{q * quaternion{-90.0, vector3d::up()}}.get_normalised();
        orientation = target_orientation;
        is_switched = true;
    } else if (input::key_states[X]) {
        target_orientation = quaternion{q * quaternion{180.0, vector3d::up()}}.get_normalised();
        orientation = target_orientation;
        is_switched = true;
    } else {
        target_orientation = q;
        is_switched = false;
    }
}

void game_camera::update() {
    t = t < 1.0 ? t + CAMERA_LAG * global::d_time : t;
    t = t > 1.0 ? 1.0 : t;
    position = vector3d::lerp(position, target_position, t);
    orientation = quaternion::slerp(orientation, target_orientation, t);
}
