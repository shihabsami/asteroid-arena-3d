#ifndef VECTOR_3D_H
#define VECTOR_3D_H

class vector3d
{
public:
    vector3d();

    vector3d(const double& x, const double& y, const double& z);

    // normalize the vector input.e. having magnitude of 1
    vector3d get_normalized() const;

    // distance between two vectors
    double get_distance(const vector3d& v) const;

    // removes the use of the expensive std::sqrt() function
    double get_distance_squared(const vector3d& v) const;

    // double get_angle();

    // operator overloads to do scalar operations
    vector3d operator+(const vector3d& v) const;

    vector3d operator-(const vector3d& v) const;

    vector3d operator*(const double& d) const;

    vector3d operator/(const double& d) const;

    double x;
    double y;
    double z;
    double m;
};

#endif // !VECTOR_3D_H
