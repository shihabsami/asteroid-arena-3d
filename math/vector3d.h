#ifndef VECTOR_3D_H
#define VECTOR_3D_H

class quaternion;

class vector3d {
public:
    vector3d();

    vector3d(double x, double y, double z);

    vector3d(const vector3d& v);

    vector3d& operator=(const vector3d& v);

    // normalise the vector i.e. having magnitude of 1.0
    void normalise();

    // get this vector normalised
    [[nodiscard]] vector3d get_normalised() const;

    [[nodiscard]] double get_magnitude() const;

    // distance between two vectors
    [[nodiscard]] double get_distance(const vector3d& v) const;

    // removes the use of the expensive sqrt() function
    [[nodiscard]] double get_distance_squared(const vector3d& v) const;

    // dot product operation between two vectors
    [[nodiscard]] double dot(const vector3d& v) const;

    // cross product operation between two vectors
    [[nodiscard]] vector3d cross(const vector3d& v) const;

    [[nodiscard]] vector3d get_rotated(double angle, const vector3d& axis) const;

    [[nodiscard]] vector3d get_rotated(const quaternion& q) const;

    void rotate(double angle, const vector3d& axis);

    void rotate(const quaternion& q);

    static vector3d lerp(const vector3d& start, const vector3d& end, double t);

    // operator overloads to do scalar operations
    vector3d operator+(const vector3d& v) const;

    vector3d operator-(const vector3d& v) const;

    vector3d operator*(double s) const;

    vector3d operator/(double s) const;

    vector3d& operator+=(const vector3d& v);

    vector3d& operator-=(const vector3d& v);

    vector3d& operator*=(double s);

    vector3d& operator/=(double s);

    static vector3d forward();

    static vector3d right();

    static vector3d up();

    double x;
    double y;
    double z;
    double m;
};

#endif // !VECTOR_3D_H
