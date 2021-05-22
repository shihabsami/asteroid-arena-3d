#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "../math/vector3d.h"
#include "../math/quaternion.h"

class game_camera {
public:
    game_camera(const vector3d& position, const quaternion& orientation);
    void look_at(const vector3d& target) const;
    void move(const vector3d& v);
    void rotate(const quaternion& q);
    void update();

    vector3d position;
    vector3d target_position;
    quaternion orientation;
    quaternion target_orientation;
    double t{};
};

#endif // !GAME_CAMERA_H
