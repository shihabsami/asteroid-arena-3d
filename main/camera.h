#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vector3d.h"
#include "../math/quaternion.h"

class camera {
public:
    camera(const vector3d& position, const vector3d& up);
    void look_at(const vector3d& target) const;
    void rotate(const quaternion& q_delta);

    vector3d position;
    vector3d up;
    quaternion l_q_delta;
    double t;
};

#endif // !CAMERA_H
