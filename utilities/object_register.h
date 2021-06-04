#ifndef OBJECT_REGISTER_H
#define OBJECT_REGISTER_H

#include "../main/global.h"
#include "../main/graphics.h"
#include "../main/game_camera.h"
#include "../objects/game_arena.h"
#include "../objects/spaceship.h"
#include "../objects/asteroid.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class object_register {
public:
    object_register();

    ~object_register();

    [[nodiscard]] bool ready() const;

    void init_camera();

    void delete_camera();

    void init_arena();

    void delete_arena();

    void init_spaceship();

    void delete_spaceship();

    void add_asteroid();

    void delete_asteroid(int index);

    vector3d next_launch_position();

    void generate_launch_positions(int total);

    static void draw_skybox();

    void update_objects();

    void draw_objects();

    void delete_objects();

    GLenum global_light;
    GLfloat* global_light_position;
    shared_ptr<game_camera> camera;
    shared_ptr<game_arena> arena;
    shared_ptr<spaceship> ship;
    vector<shared_ptr<asteroid>> asteroids;
    vector<vector3d> launch_positions;
};

#endif // !OBJECT_REGISTER_H
