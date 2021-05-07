#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3d.h"

#include <array>

using std::array;

class quaternion {
public:
    quaternion();
    quaternion(const double& s, const vector3d& v);
    quaternion(const quaternion& other);
    quaternion& operator=(const quaternion& other);
    quaternion operator*(const quaternion& other) const;
    void normalise();
    array<double, 16> to_matrix() const;

    double s;
    vector3d v;
};

#endif // !QUATERNION_H
