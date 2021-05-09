#include "quaternion.h"

#include <cmath>

quaternion::quaternion() = default;

quaternion::quaternion(const double& w, const vector3d& v)
    : w(w), v(v) {
}

//quaternion::quaternion(const double& w, const double& x, const double& y, const double& z)
//    : w(w), x(x), y(y), z(z) {
//}

void quaternion::normalise() {
    double m = get_magnitude();
    w /= m;
    v /= m;

}

quaternion quaternion::operator*(const quaternion& q) const {
    return {
        w * q.w - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z, {
            w * q.v.x + v.x * q.w + v.y * q.v.z - v.z * q.v.y,
            w * q.v.y - v.x * q.v.z + v.y * q.w + v.z * q.v.x,
            w * q.v.z + v.x * q.v.y - v.y * q.v.x + v.z * q.w
        }
    };

    // return { w * q.w - v.dot(q.v), q.v * w + v * q.w + v.cross(q.v) };
}

double* quaternion::to_matrix() const {
    auto* matrix = new double[16]{
        1 - 2 * (v.z * v.z + v.y * v.y), 2 * (v.x * v.y + w * v.z), 2 * (v.z * v.x - w * v.y), 0.0,
        2 * (v.x * v.y - w * v.z), 1 - 2 * (v.x * v.x + v.z * v.z), 2 * (v.y * v.z + w * v.x), 0.0,
        2 * (v.z * v.x + w * v.y), 2 * (v.y * v.z - w * v.x), 1 - 2 * (v.x * v.x + v.y * v.y), 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    return matrix;
}

double quaternion::get_magnitude() const {
    return sqrt(w * w + v.x * v.x + v.y * v.y + v.z * v.z);
}

quaternion::quaternion(const quaternion& q) = default;

quaternion& quaternion::operator=(const quaternion& q) = default;

