#include "quaternion.h"

#include <cmath>

quaternion::quaternion() = default;

quaternion::quaternion(const double& s, const vector3d& v)
    : s(s), v(v) {}

void quaternion::normalise() {
    double m = sqrt(s * s + v.x * v.x + v.y + v.y + v.z + v.z);
    s /= m;
    v /= m;
}

quaternion quaternion::operator*(const quaternion& q) const {
    return {
        s * q.s - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z, {
            s * q.v.x + v.x * q.s + v.y * q.v.z - v.z * q.v.y,
            s * q.v.y - v.x * q.v.z + v.y * q.s + v.z * q.v.x,
            s * q.v.z + v.x * q.v.y - v.y * q.v.x + v.z * q.s }};
}

array<double, 16> quaternion::to_matrix() const {
    array<double, 16> matrix{};
    matrix.at(0) = 1 - 2 * v.y * v.y - 2 * v.z * v.z;
    matrix.at(1) = 2 * v.x * v.y - 2 * s * v.z;
    matrix.at(2) = 2 * v.x * v.z + 2 * s * v.y;
    matrix.at(3) = 0;

    matrix.at(4) = 2 * v.x * v.y + 2 * s * v.z;
    matrix.at(5) = 1 - 2 * v.x * v.x - 2 * v.z * v.z;
    matrix.at(6) = 2 * v.y * v.z + 2 * s * v.x;
    matrix.at(7) = 0;

    matrix.at(8) = 2 * v.x * v.z - 2 * s * v.y;
    matrix.at(9) = 2 * v.y * v.z - 2 * s * v.x;
    matrix.at(10) = 1 - 2 * v.x * v.x - 2 * v.y * v.y;
    matrix.at(11) = 0;

    matrix.at(11) = 0;
    matrix.at(12) = 0;
    matrix.at(13) = 0;
    matrix.at(14) = 1;

    return matrix;
}

quaternion::quaternion(const quaternion& other) = default;

quaternion& quaternion::operator=(const quaternion& other) = default;

