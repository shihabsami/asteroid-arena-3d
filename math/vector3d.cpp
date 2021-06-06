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
    return std::sqrt(x * x + y * y + z * z);
}

void vector3d::normalise() {
    x /= m;
    y /= m;
    z /= m;
    m = get_magnitude();
}

vector3d vector3d::get_normalised() const {
    return { x / m, y / m, z / m };
}

double vector3d::get_distance(const vector3d& v) const {
    return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
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

vector3d vector3d::get_rotated(double angle, const vector3d& axis) const {
    return get_rotated({ angle, axis });
}

vector3d vector3d::get_rotated(const quaternion& q) const {
    vector3d u(q.x, q.y, q.z);
    double s = q.w;
    return (u * 2.0 * dot(u)) + *this * (s * s - u.dot(u)) + (u.cross(*this) * 2.0f * s);
}

void vector3d::rotate(double angle, const vector3d& axis) {
    vector3d rotated = get_rotated(angle, axis);
    x = rotated.x;
    y = rotated.y;
    z = rotated.z;
    m = rotated.m;
}

void vector3d::rotate(const quaternion& q) {
    vector3d rotated = get_rotated(q);
    x = rotated.x;
    y = rotated.y;
    z = rotated.z;
    m = rotated.m;
}

vector3d vector3d::lerp(const vector3d& start, const vector3d& end, double t) {
    return {
        std::lerp(start.x, end.x, t),
        std::lerp(start.y, end.y, t),
        std::lerp(start.z, end.z, t)
    };
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

vector3d vector3d::forward() { return { 0.0, 0.0, -1.0 }; }

vector3d vector3d::right() { return { 1.0, 0.0, 0.0 }; }

vector3d vector3d::up() { return { 0.0, 1.0, 0.0 }; }
