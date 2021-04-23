#include "vector3d.h"

#include <cmath>

vector3d::vector3d()
    : x(), y(), z(), m()
{}

vector3d::vector3d(const double& x, const double& y, const double& z)
    : x(x), y(y), z(z)
{
    m = std::sqrt(x * x + y * y + z * z);
}

vector3d vector3d::get_normalized() const
{
    return { x / m, y / m, z / m };
}

// the angle of the vector
//double vector3d::get_angle()
//{
//    return atan2(y, x) * (180 / M_PI);
//}

double vector3d::get_distance(const vector3d& v) const
{
    return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

double vector3d::get_distance_squared(const vector3d& v) const
{
    return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

vector3d vector3d::operator+(const vector3d& v) const
{
    return { x + v.x, y + v.y, z + v.z };
}

vector3d vector3d::operator-(const vector3d& v) const
{
    return { x - v.x, y - v.y, z - v.z };
}

vector3d vector3d::operator*(const double& d) const
{
    return { x * d, y * d, z * d };
}

vector3d vector3d::operator/(const double& d) const
{
    return { x / d, y / d, z / d };
}
