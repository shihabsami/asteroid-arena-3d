#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

class object_register;
class collision_handler;

#include <memory>

using std::shared_ptr;
using std::make_shared;

class game_manager {
public:
    game_manager();

    ~game_manager();

    void start_game();

    void end_game();

    void update();

    void display();

    double start_time;
    double l_wave_time;
    int wave_number;
    bool game_started;
    bool game_running;
    bool game_over;

    shared_ptr<object_register> o_register;
    shared_ptr<collision_handler> c_handler;
};

#endif // !GAME_MANAGER_H
