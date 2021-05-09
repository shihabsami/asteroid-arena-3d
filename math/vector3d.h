#ifndef VECTOR_3D_H
#define VECTOR_3D_H

class quaternion;

class vector3d {
public:
    vector3d();

    vector3d(double x, double y, double z);

    vector3d(const vector3d& v);

    vector3d& operator=(const vector3d& v);

    // normalize the vector i.e. having magnitude of 1.0
    void normalise();

    [[nodiscard]] double get_magnitude() const;

    // distance between two vectors
    [[nodiscard]] double get_distance(const vector3d& v) const;

    // removes the use of the expensive sqrt() function
    [[nodiscard]] double get_distance_squared(const vector3d& v) const;

    // dot product operation between two vectors
    [[nodiscard]] double dot(const vector3d& v) const;

    // cross product operation between two vectors
    [[nodiscard]] vector3d cross(const vector3d& v) const;

    void rotate(double angle, const vector3d& axis);

    void rotate(const quaternion& q);

    // operator overloads to do scalar operations
    vector3d operator+(const vector3d& v) const;

    vector3d operator-(const vector3d& v) const;

    vector3d operator*(double s) const;

    vector3d operator/(double s) const;

    vector3d& operator+=(const vector3d& v);

    vector3d& operator-=(const vector3d& v);

    vector3d& operator*=(double s);

    vector3d& operator/=(double s);

    double x;
    double y;
    double z;
    double m;
};

#endif // !VECTOR_3D_H
