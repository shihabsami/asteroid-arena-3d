#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vector3d.h"

class camera {
public:
    camera(const vector3d& position, const vector3d& up_vector);

    vector3d position;
    vector3d up_vector;
};

#endif // !CAMERA_H
