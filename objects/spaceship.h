#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "game_object.h"

class spaceship : public game_object {
public:
    spaceship(
        shared_ptr<mesh_t>& body, shared_ptr<mesh_t>& l_wing, shared_ptr<mesh_t>& r_wing,
        shared_ptr<vector3d>& position, shared_ptr<quaternion>& rotation, shared_ptr<vector3d>& scale,
        shared_ptr<vector3d>& direction, double velocity);

    void init() override;

    void movement() override;

    void pitch(bool negative, double amount);

    void roll(bool negative, double amount);

    void yaw(bool negative, double amount);

    void draw() override;

    double t{};
    bool is_moving{};
};

#endif // !SPACESHIP_H
