#include "quaternion.h"
#include "vector3d.h"
#include "game_math.h"

using std::abs;

quaternion::quaternion() : w(1.0), x(), y(), z() {}

quaternion::quaternion(double angle, const vector3d& axis) {
    double angle_radians = to_radians(angle * 0.5);
    w = cos(angle_radians);
    x = axis.x * sin(angle_radians) / axis.m;
    y = axis.y * sin(angle_radians) / axis.m;
    z = axis.z * sin(angle_radians) / axis.m;
}

quaternion::quaternion(double w, double x, double y, double z)
    : w(w), x(x), y(y), z(z) {}

quaternion::quaternion(const quaternion& q) = default;

quaternion& quaternion::operator=(const quaternion& q) = default;

quaternion quaternion::get_normalised() const {
    double n = get_norm();
    return { w / n, x / n, y / n, z / n };
}

double quaternion::get_norm() const {
    return sqrt(w * w + x * x + y * y + z * z);
}

double* quaternion::to_matrix() const {
    auto* matrix = new double[16]{
        1 - 2 * (z * z + y * y), 2 * (x * y + w * z), 2 * (z * x - w * y), 0.0,
        2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x), 0.0,
        2 * (z * x + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y), 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    return matrix;
}

quaternion quaternion::operator*(const quaternion& q) const {
    return {
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    };
}

quaternion quaternion::operator*(double s) const {
    return { w * s, x * s, y * s, z * s };
}

quaternion quaternion::get_conjugate() const {
    return { w, -x, -y, -z };
}

quaternion quaternion::get_inverse() const {
    double n = get_norm();
    return get_conjugate() * (n * n);
}

quaternion quaternion::slerp(const quaternion& start, const quaternion& end, double t) {
    quaternion q{};
    double cos_half_theta = start.w * end.w + start.x * end.x + start.y * end.y + start.z * end.z;
    if (abs(cos_half_theta) >= 1.0) {
        q.w = start.w;
        q.x = start.x;
        q.y = start.y;
        q.z = start.z;

        return q;
    }

    quaternion end_mod = end;
    if (cos_half_theta < 0) {
        end_mod.w = -end_mod.w;
        end_mod.x = -end_mod.x;
        end_mod.y = -end_mod.y;
        cos_half_theta = -cos_half_theta;
    }

    double sin_half_theta = sqrt(1.0 - cos_half_theta * cos_half_theta);
    if (fabs(sin_half_theta) < 0.001) {
        q.w = (start.w * 0.5 + end_mod.w * 0.5);
        q.x = (start.x * 0.5 + end_mod.x * 0.5);
        q.y = (start.y * 0.5 + end_mod.y * 0.5);
        q.z = (start.z * 0.5 + end_mod.z * 0.5);

        return q;
    }

    double half_theta = acos(cos_half_theta);
    double ratio_a = sin((1 - t) * half_theta) / sin_half_theta;
    double ratio_b = sin(t * half_theta) / sin_half_theta;
    q.w = (start.w * ratio_a + end_mod.w * ratio_b);
    q.x = (start.x * ratio_a + end_mod.x * ratio_b);
    q.y = (start.y * ratio_a + end_mod.y * ratio_b);
    q.z = (start.z * ratio_a + end_mod.z * ratio_b);

    return q;
}
