#include "game_camera.h"
#include "config.h"

game_camera::game_camera(const vector3d& position, const quaternion& orientation)
    : position(position), target_position(position), orientation(orientation), target_orientation(orientation) {}

void game_camera::look_at(const vector3d& target) const {
    vector3d up{ 0.0, 1.0, 0.0 };
    vector3d up_oriented{ up.get_rotated(orientation) };
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        position.x, position.y, position.z,
        target.x, target.y, target.z,
        up_oriented.x, up_oriented.y, up_oriented.z);


//    printf("s_position: q.x: %.3f, q.y: %.3f, q.z: %.3f\n", position.x, position.y, position.z);
//    printf("t_position: q.x: %.3f, q.y: %.3f, q.z: %.3f\n", target.x, target.y, target.z);
//    printf("s_up: q.x: %.3f, q.y: %.3f, q.z: %.3f\n\n", up_oriented.x, up_oriented.y, up_oriented.z);
//
//    printf("s_orientation: q.w: %.3f, q.x: %.3f, q.y: %.3f, q.z: %.3f\n", orientation.w, orientation.x, orientation.y, orientation.z);
//    printf("t_orientation: q.w: %.3f, q.x: %.3f, q.y: %.3f, q.z: %.3f\n\n", target_orientation.w, target_orientation.x, target_orientation.y, target_orientation.z);
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
    if (t > 1.0) t = 1.0;
    position = vector3d::lerp(position, target_position, t);
    orientation = quaternion::slerp(orientation, target_orientation, t);
}
