#include "object_register.h"
#include "input_handler.h"
#include "asset_library.h"

object_register::object_register() {
    directional_light = GL_LIGHT0;
    directional_light_position = new GLfloat[4]{ -0.8, 0.8, -0.8, 0.0 };
    set_light(directional_light, material::light, directional_light_position);

    init_camera();
    init_arena();
    init_spaceship();
}

object_register::~object_register() {
    camera.reset();
    arena.reset();
    ship.reset();
    p_system.reset();

    delete[] directional_light_position;
}

void object_register::init_camera() {
    camera = make_shared<game_camera>(
        vector3d{ 0.0, 0.0, TOTAL_UNITS / 2.0 }, quaternion{});
}

void object_register::init_arena() {
    arena = make_shared<game_arena>(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
}

void object_register::init_spaceship() {
    ship = make_shared<spaceship>(
        asset_library::models.at(SPACESHIP_BODY_MODEL_KEY),
        asset_library::models.at(SPACESHIP_L_WING_MODEL_KEY),
        asset_library::models.at(SPACESHIP_R_WING_MODEL_KEY),
        asset_library::textures.at(SPACESHIP_TEX_KEY),
        vector3d{}, quaternion{},
        vector3d{ SPACESHIP_SCALE, SPACESHIP_SCALE, SPACESHIP_SCALE },
        vector3d::forward(), SPACESHIP_VELOCITY
    );

    ship->init();
}

void object_register::add_asteroid() {
    vector3d position = next_launch_position();
    vector3d direction = (ship->position - position).get_normalised();
    double scale = get_random(ASTEROID_MIN_SCALE, ASTEROID_MAX_SCALE);
    double velocity = get_random(ASTEROID_MIN_VELOCITY, ASTEROID_MAX_VELOCITY);
    shared_ptr<asteroid> a = make_shared<asteroid>(
        asset_library::models.at(ASTEROID_MODEL_KEY),
        asset_library::textures.at(asset_library::asteroid_tex_key(get_random(1, 6))),
        position, quaternion{}, vector3d{ scale, scale, scale },
        direction, velocity
    );

    a->init();
    asteroids.push_back(a);
}

void object_register::delete_asteroid(size_t index) {
    auto a = asteroids.begin() + (signed) index;
    p_system->add_explosion(*a);
    asteroids.erase(a);
}

vector3d object_register::next_launch_position() {
    vector3d position = launch_positions.at(launch_positions.size() - 1);
    launch_positions.pop_back();
    return position;
}

void object_register::generate_launch_positions(int total) {
    launch_positions.clear();
    double r = sqrt(
        arena->width * arena->width + arena->height * arena->height + arena->length * arena->length);

    for (int i = 0; i < total; ++i) {
        double theta = get_random(0.0, M_PI * 2);
        double phi = acos((2 * get_random(0.0, 1.0)) - 1);
        launch_positions.emplace_back(
            r * sin(phi) * cos(theta),
            r * sin(phi) * sin(theta),
            r * cos(phi)
        );
    }
}

void object_register::add_bullet() {
    p_system->add_bullet(ship);
}

void object_register::delete_bullet(size_t index) {
    p_system->bullet_particles.erase(p_system->bullet_particles.begin() + (signed) index);
}

void object_register::draw_skybox() {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    auto draw_quad = [] {
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(-1.0, -1.0, -1.0);
        glTexCoord2d(1.0, 0.0);
        glVertex3d(1.0, -1.0, -1.0);
        glTexCoord2d(1.0, 1.0);
        glVertex3d(1.0, 1.0, -1.0);
        glTexCoord2d(0.0, 1.0);
        glVertex3d(-1.0, 1.0, -1.0);
        glEnd();
    };

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_FRONT_TEX_KEY));
    glPushMatrix();
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_BACK_TEX_KEY));
    glPushMatrix();
    glRotated(180.0, 1.0, 0.0, 0.0);
    glRotated(180.0, 0.0, 0.0, 1.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_TOP_TEX_KEY));
    glPushMatrix();
    glRotated(90.0, 1.0, 0.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_BOTTOM_TEX_KEY));
    glPushMatrix();
    glRotated(270.0, 1.0, 0.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_LEFT_TEX_KEY));
    glPushMatrix();
    glRotated(90.0, 0.0, 1.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,
        asset_library::textures.at(SKYBOX_RIGHT_TEX_KEY));
    glPushMatrix();
    glRotated(270.0, 0.0, 1.0, 0.0);
    draw_quad();
    glPopMatrix();

    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
}

void object_register::draw() {
    camera->orient();
    draw_skybox();
    camera->translate();

    arena->draw();
    if (!camera->is_switched)
        ship->draw();

    for (const auto& a : asteroids)
        a->draw();

    set_light(directional_light, material::light, directional_light_position);
    p_system->draw_particles();
}

void object_register::update() {
    ship->movement();
    camera->move(ship->position);
    camera->rotate(ship->rotation);
    camera->update();

    for (const auto& a : asteroids)
        a->movement();

    p_system->update(camera);
    if ((global::c_time - l_fire_time >= FIRING_INTERVAL) && input::mouse_states[CLICK]) {
        add_bullet();
        l_fire_time = global::c_time;
    }
}

void object_register::reset() {
    ship->position = vector3d{};
    ship->rotation = quaternion{};
    ship->direction = vector3d::forward();
    ship->t = 0.0;

    camera->position = ship->position - ship->direction * CAMERA_Z_OFFSET;
    camera->orientation = quaternion{};

    asteroids.clear();
    p_system->clear();

    input::x_delta = 0.0;
    input::y_delta = 0.0;
}
