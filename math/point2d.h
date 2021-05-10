#ifndef POINT_2D_H
#define POINT_2D_H

class point2d
{
public:
    point2d();

    point2d(double x, double y);

    double get_distance(const point2d& p);

    double x;
    double y;
};

#endif // !POINT_2D_H
