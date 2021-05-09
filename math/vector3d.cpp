#include "vector3d.h"
#include "quaternion.h"
#include "game_math.h"

vector3d::vector3d()
    : x(), y(), z(), m() {}

vector3d::vector3d(double x, double y, double z)
    : x(x), y(y), z(z) {
    m = get_magnitude();
}

vector3d::vector3d(const vector3d& v) = default;

vector3d& vector3d::operator=(const vector3d& v) = default;

double vector3d::get_magnitude() const {
    return sqrt(x * x + y * y + z * z);
}

void vector3d::normalise() {
    x /= m;
    y /= m;
    z /= m;
    m = get_magnitude();
}

double vector3d::get_distance(const vector3d& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

double vector3d::get_distance_squared(const vector3d& v) const {
    return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

double vector3d::dot(const vector3d& v) const {
    return x * v.x + y * v.y + z * v.z;
}

vector3d vector3d::cross(const vector3d& v) const {
    return {
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    };
}

void vector3d::rotate(double angle, const vector3d& axis) {
    quaternion p{ 0.0, *this };
    quaternion q{ cos(to_radians(angle  * 0.5)), axis * sin(to_radians(angle  * 0.5)) };
    quaternion rotated = q * p * q.get_inverse();

    x = rotated.x;
    y = rotated.y;
    z = rotated.z;
    m = get_magnitude();
}

void vector3d::rotate(const quaternion& q) {
    vector3d u(q.x, q.y, q.z);
    double s = q.w;

    vector3d rotate = (u * 2.0 * dot(u)) + *this * (s * s - u.dot(u)) + (u.cross(*this) * 2.0f * s);
    x = rotate.x;
    y = rotate.y;
    z = rotate.z;
    m = rotate.m;
}

vector3d vector3d::operator+(const vector3d& v) const {
    return { x + v.x, y + v.y, z + v.z };
}

vector3d vector3d::operator-(const vector3d& v) const {
    return { x - v.x, y - v.y, z - v.z };
}

vector3d vector3d::operator*(double s) const {
    return { x * s, y * s, z * s };
}

vector3d vector3d::operator/(double s) const {
    return { x / s, y / s, z / s };
}

vector3d& vector3d::operator+=(const vector3d& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    m = get_magnitude();
    return *this;
}

vector3d& vector3d::operator-=(const vector3d& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    m = get_magnitude();
    return *this;
}

vector3d& vector3d::operator*=(double s) {
    x *= s;
    y *= s;
    z *= s;
    m = get_magnitude();
    return *this;
}

vector3d& vector3d::operator/=(double s) {
    x /= s;
    y /= s;
    z /= s;
    m = get_magnitude();
    return *this;
}
