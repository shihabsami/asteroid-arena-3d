#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3d.h"

#include <array>

using std::array;

class quaternion {
public:
    quaternion();

    quaternion(double w, const vector3d& v);

    quaternion(double w, double x, double y, double z);

    quaternion(const quaternion& q);

    quaternion& operator=(const quaternion& q);

    void normalise();

    [[nodiscard]] double get_norm() const;

    [[nodiscard]] double* to_matrix() const;

    [[nodiscard]] quaternion get_conjugate() const;

    [[nodiscard]] quaternion get_inverse() const;

    quaternion operator*(const quaternion& q) const;

    quaternion operator*(double s) const;

    double w{};
    double x{};
    double y{};
    double z{};
    double n{};
};

#endif // !QUATERNION_H
