#include "game_manager.h"
#include "../main/config.h"
#include "../main/global.h"
#include "../utilities/object_register.h"
#include "../utilities/collision_handler.h"

game_manager::game_manager() {
    start_time = 0.0;
    l_wave_time = 0.0;
    wave_number = 1;
    game_started = false;
    game_running = false;
    game_over = false;
}

game_manager::~game_manager() {
    o_register.reset();
    c_handler.reset();
}

void game_manager::start_game() {
    start_time = global::c_time;
    l_wave_time = start_time;
    wave_number = 1;
    game_started = true;
    game_running = true;
    game_over = false;

    o_register->init_arena();
    o_register->init_spaceship();
}

void game_manager::end_game() {
    game_running = false;
    game_over = true;

    o_register->delete_arena();
    o_register->delete_spaceship();
    o_register->delete_objects();
}

void game_manager::display() {
    c_handler->run();
    o_register->draw_objects();

    if (game_running) {
        if (global::c_time - l_wave_time >= WAVE_INTERVAL) {
            if (o_register->asteroids.size() < MAX_ASTEROID_COUNT) {
                o_register->generate_launch_positions(wave_number);
                for (int i = 0; i < wave_number; ++i)
                    o_register->add_asteroid();

                ++wave_number;
            }

            l_wave_time = global::c_time;
        }
    }

    error_check("game_manager::draw_objects");
}

void game_manager::update() {
    o_register->update_objects();
}
