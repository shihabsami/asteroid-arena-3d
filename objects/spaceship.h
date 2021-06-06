#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "game_object.h"

class spaceship : public game_object {
public:
    spaceship(
        const shared_ptr<mesh_t>& body,  const shared_ptr<mesh_t>& l_wing, const shared_ptr<mesh_t>& r_wing, GLuint texture_id,
        const vector3d& position, const quaternion& rotation, const vector3d& scale,
        const vector3d& direction, double velocity);

    ~spaceship() override;

    void init() override;

    void movement() override;

    void pitch(double amount);

    void roll(double amount);

    void yaw(double amount);

    void draw() override;

    GLenum point_light;
    shared_ptr<mesh_t> l_wing;
    shared_ptr<mesh_t> r_wing;
    quaternion l_wing_rotation;
    quaternion r_wing_rotation;

    bool test{};
    double t{};
};

#endif // !SPACESHIP_H
