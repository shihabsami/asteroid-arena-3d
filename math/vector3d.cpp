#include "vector3d.h"

#include <cmath>

vector3d::vector3d()
    : x(), y(), z(), m() {}

vector3d::vector3d(const double& x, const double& y, const double& z)
    : x(x), y(y), z(z) {
    m = sqrt(x * x + y * y + z * z);
}

vector3d::vector3d(const vector3d& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    m = v.m;
}

vector3d& vector3d::operator=(const vector3d& v) = default;

void vector3d::normalise() {
    x /= m;
    y /= m;
    z /= m;
}

double vector3d::get_distance(const vector3d& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

double vector3d::get_distance_squared(const vector3d& v) const {
    return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

vector3d vector3d::operator+(const vector3d& v) const {
    return { x + v.x, y + v.y, z + v.z };
}

vector3d vector3d::operator-(const vector3d& v) const {
    return { x - v.x, y - v.y, z - v.z };
}

vector3d vector3d::operator*(const double& d) const {
    return { x * d, y * d, z * d };
}

vector3d vector3d::operator/(const double& d) const {
    return { x / d, y / d, z / d };
}

double vector3d::dot(const vector3d& v) const {

    return x * v.x + y * v.y + z * v.z;
}

vector3d vector3d::cross(const vector3d& v) const {
    return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
}
