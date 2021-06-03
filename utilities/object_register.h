#ifndef OBJECT_REGISTER_H
#define OBJECT_REGISTER_H

#include "../main/graphics.h"
#include "../main/global.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class game_camera;
class game_arena;
class spaceship;
class asteroid;
class vector3d;

class object_register {
public:
    object_register();

    ~object_register();

    void init_camera();

    void delete_camera();

    void init_arena();

    void delete_arena();

    void init_spaceship();

    void delete_spaceship();

    void add_asteroid();

    void delete_asteroid(int index);

    vector3d next_launch_position();

    void generate_launch_positions(int how_many);

    static void draw_skybox();

    void update();

    void display();

    GLenum global_light;
    GLfloat* global_light_position;
    shared_ptr<game_camera> camera;
    shared_ptr<game_arena> arena;
    shared_ptr<spaceship> ship;
    vector<shared_ptr<asteroid>> asteroids;
    vector<vector3d> launch_positions;
};

#endif // !OBJECT_REGISTER_H
