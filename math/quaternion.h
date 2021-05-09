#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3d.h"

#include <array>

using std::array;

class quaternion {
public:
    quaternion();
    quaternion(const double& w, const vector3d& v);
    quaternion(const double& w, const double& x, const double& y, const double& z);
    quaternion(const quaternion& q);
    quaternion& operator=(const quaternion& q);
    quaternion operator*(const quaternion& q) const;
    void normalise();
    [[nodiscard]] double* to_matrix() const;

    double get_magnitude() const;

    double w;
    vector3d v;
};

#endif // !QUATERNION_H
