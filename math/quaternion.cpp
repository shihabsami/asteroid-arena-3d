#include "quaternion.h"

#include <cmath>

quaternion::quaternion() = default;

quaternion::quaternion(double w, const vector3d& v)
    : w(w), x(v.x), y(v.y), z(v.z) {
    n = get_norm();
}

quaternion::quaternion(double w, double x, double y, double z)
    : w(w), x(x), y(y), z(z) {
    n = get_norm();
}

quaternion::quaternion(const quaternion& q) = default;

quaternion& quaternion::operator=(const quaternion& q) = default;

void quaternion::normalise() {
    w /= n;
    x /= n;
    y /= n;
    z /= n;
}

double quaternion::get_norm() const {
    return sqrt(w * w + x * x + y * y + z * z);
}

double* quaternion::to_matrix() const {
    auto* matrix = new double[16]{
        1 - 2 * (z * z + y * y), 2 * (x * y + w * z), 2 * (z * x - w * y), 0.0,
        2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x), 0.0,
        2 * (z * x + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y), 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    return matrix;
}

quaternion quaternion::operator*(const quaternion& q) const {
    return {
        w * q.w - x * q.x - y * q.y - z * q.z, {
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        }
    };
}

quaternion quaternion::operator*(double s) const {
    return { w * s, x * s, y * s, z * s };
}

quaternion quaternion::get_conjugate() const {
    return { w, -x, -y, -z };
}

quaternion quaternion::get_inverse() const {
    return get_conjugate() * (1 / n * n);
}
