#ifndef POINT_3D_H
#define POINT_3D_H

class point3d
{
public:
    point3d();

    point3d(const double& x, const double& y, const double& z);

    double get_distance(const point3d& p) const;

    double x;
    double y;
    double z;
};

#endif // !POINT_3D_H
