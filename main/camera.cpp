#include "camera.h"
#include "graphics.h"
#include "config.h"

camera::camera(const vector3d& position, const vector3d& up)
    : position(position), up(up), t(0.0) {}

void camera::look_at(const vector3d& target) const {
    gluLookAt(
        position.x, position.y, position.z,
        target.x, target.y, target.z,
        up.x, up.z, up.z);
}

void camera::rotate(const quaternion& q_delta) {
//    quaternion difference = l_q_delta * q_delta.get_inverse();
//    if (difference.width != 0.0 && difference.x != 0.0 && difference.y != 0.0 && difference.z != 0.0) {
//        t = (t < 1.0) ? t + CAMERA_LAG * g::d_time : 1.0;
//    } else
//        t = 0.0;
//
//    l_q_delta = quaternion::slerp(l_q_delta, q_delta, 1.0);

    up.rotate(q_delta);
}
