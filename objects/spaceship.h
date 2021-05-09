#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "game_object.h"

class spaceship : public game_object {
public:
    using game_object::game_object;

    void init() override;

    void movement() override;

    void draw() override;

    double t;
    bool is_moving;
};

#endif // !SPACESHIP_H
