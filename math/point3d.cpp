#include "point3d.h"

#include <cmath>

point3d::point3d()
    : x(), y()
{}

point3d::point3d(const double& x, const double& y, const double& z)
    : x(x), y(y), z(z)
{}

double point3d::get_distance(const point3d& p) const
{
    // distance between two points
    return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
}
