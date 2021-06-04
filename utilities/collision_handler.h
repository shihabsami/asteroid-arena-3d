#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

class game_manager;
class object_register;

#include <memory>

using std::shared_ptr;
using std::make_shared;

class collision_handler {
public:
    explicit collision_handler(const shared_ptr<game_manager>& manager);

    void run();

    void ship_against_wall();

    void ship_against_asteroid();

    void bullet_against_wall();

    void bullet_against_asteroid();

    void asteroid_against_wall();

    void asteroid_against_asteroid();

    shared_ptr<game_manager> manager;
    shared_ptr<object_register> o_register;
};

#endif // !COLLISION_HANDLER_H
