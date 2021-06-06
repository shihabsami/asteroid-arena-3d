#ifndef OBJECT_REGISTER_H
#define OBJECT_REGISTER_H

#include "../main/global.h"
#include "../main/graphics.h"
#include "../main/game_camera.h"
#include "../objects/game_arena.h"
#include "../objects/spaceship.h"
#include "../objects/asteroid.h"
#include "../utilities/particle_system.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class object_register {
public:
    object_register();

    ~object_register();

    void init_camera();

    void init_arena();

    void init_spaceship();

    void add_asteroid();

    void delete_asteroid(size_t index);

    void add_bullet();

    void delete_bullet(size_t index);

    vector3d next_launch_position();

    void generate_launch_positions(int total);

    static void draw_skybox();

    void draw();

    void update();

    void reset();

    GLenum directional_light;
    GLfloat* directional_light_position;
    double l_fire_time{};
    bool is_ready{};

    shared_ptr<game_camera> camera;
    shared_ptr<game_arena> arena;
    shared_ptr<spaceship> ship;
    vector<shared_ptr<asteroid>> asteroids;
    vector<vector3d> launch_positions;
    shared_ptr<particle_system> p_system;
};

#endif // !OBJECT_REGISTER_H
