#ifndef GLOBAL_H
#define GLOBAL_H

class global {
public:
    static double w_width;
    static double w_height;
    static double d_time;
    static double c_time;

    static shared_ptr<vector3d> x_axis;
    static shared_ptr<vector3d> y_axis;
    static shared_ptr<vector3d> z_axis;
    static shared_ptr<vector3d> forward;
    static shared_ptr<quaternion> orientation;
};

double global::w_height;
shared_ptr<vector3d> global::x_axis = make_shared<vector3d>(1.0, 0.0, 0.0);
shared_ptr<vector3d> global::y_axis = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> global::z_axis = make_shared<vector3d>(0.0, 0.0, 1.0);
shared_ptr<vector3d> global::forward = make_shared<vector3d>(0.0, 0.0, -1.0);
shared_ptr<quaternion> global::orientation = make_shared<quaternion>(quaternion::get_identity());

#endif // !GLOBAL_H
