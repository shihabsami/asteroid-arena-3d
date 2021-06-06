#ifndef ASTEROID_H
#define ASTEROID_H

#include <vector>

#include "game_object.h"
#include "../math/game_math.h"

class asteroid : public game_object {
public:
    using game_object::game_object;

    void init() override;

    void movement() override;

    void draw() override;

    int hp;
    int hit_count;
    bool can_bounce;
    double d_angle;
    vector3d rotation_axis;
};

#endif // !ASTEROID_H
