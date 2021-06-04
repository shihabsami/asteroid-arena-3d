#ifndef QUATERNION_H
#define QUATERNION_H

class vector3d;

class quaternion {
public:
    quaternion();

    quaternion(double angle, const vector3d& axis);

    quaternion(double w, double x, double y, double z);

    quaternion(const quaternion& q);

    quaternion& operator=(const quaternion& q);

    [[maybe_unused]] void normalise();

    [[nodiscard]] double get_norm() const;

    [[nodiscard]] double* to_matrix() const;

    [[nodiscard]] quaternion get_conjugate() const;

    [[nodiscard]] quaternion get_inverse() const;

    quaternion operator*(const quaternion& q) const;

    quaternion operator*(double s) const;

    static quaternion slerp(const quaternion& start, const quaternion& end, double t);

    double w{};
    double x{};
    double y{};
    double z{};
};

#endif // !QUATERNION_H
