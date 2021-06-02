#ifndef OBJECT_REGISTER_H
#define OBJECT_REGISTER_H

#include "../objects/game_arena.h"
#include "../objects/spaceship.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

class object_register {
public:
    void init();

    void init_arena();

    void delete_arena();

    void init_spaceship();

    void delete_spaceship();

    shared_ptr<game_arena> arena;
    shared_ptr<spaceship> ship;
};

#endif // !OBJECT_REGISTER_H
