#include "object_register.h"
#include "asset_library.h"

#include "../main/game_camera.h"
#include "../objects/game_arena.h"
#include "../objects/spaceship.h"
#include "../objects/asteroid.h"

object_register::object_register() {
    global_light = GL_LIGHT0;
    global_light_position = new GLfloat[4]{ -0.8, 0.8, -0.8, 0.0 };
    set_light(global_light, material::light, global_light_position);

    init_camera();
    init_arena();
    init_spaceship();
}

object_register::~object_register() {
    camera.reset();
    arena.reset();
    ship.reset();
}

void object_register::init_camera() {
    camera = make_shared<game_camera>(
        vector3d{ 0.0, 0.0, TOTAL_UNITS / 2.0 }, quaternion{});
}

void object_register::delete_camera() {
    camera.reset();
}

void object_register::init_arena() {
    arena = make_shared<game_arena>(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
}

void object_register::delete_arena() {
    arena.reset();
}

void object_register::init_spaceship() {
    ship = make_shared<spaceship>(
        asset_library::models.at(SPACESHIP_BODY_MODEL_KEY),
        asset_library::models.at(SPACESHIP_L_WING_MODEL_KEY),
        asset_library::models.at(SPACESHIP_R_WING_MODEL_KEY),
        vector3d{}, quaternion{}, vector3d{SPACESHIP_SCALE, SPACESHIP_SCALE, SPACESHIP_SCALE},
        vector3d::forward(), SPACESHIP_VELOCITY
    );

    ship->init();
}

void object_register::delete_spaceship() {
    ship.reset();
}

void object_register::draw_skybox() {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    auto draw_quad = []{
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

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_FRONT_TEX_KEY));
    glPushMatrix();
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_BACK_TEX_KEY));
    glPushMatrix();
    glRotated(180.0, 1.0, 0.0, 0.0);
    glRotated(180.0, 0.0, 0.0, 1.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_TOP_TEX_KEY));
    glPushMatrix();
    glRotated(90.0, 1.0, 0.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_BOTTOM_TEX_KEY));
    glPushMatrix();
    glRotated(270.0, 1.0, 0.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_LEFT_TEX_KEY));
    glPushMatrix();
    glRotated(90.0, 0.0, 1.0, 0.0);
    draw_quad();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SKYBOX_RIGHT_TEX_KEY));
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

void object_register::display() {
    camera->orient();
    draw_skybox();
    camera->translate();

    arena->draw();

    for (const auto& a : asteroids) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(ASTEROID_TEX_KEY));
        set_material(material::white);
        a->draw();
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, asset_library::textures.at(SPACESHIP_TEX_KEY));
    set_material(material::white);
    ship->draw();
    glDisable(GL_TEXTURE_2D);

    set_light(global_light, material::light, global_light_position);
}

void object_register::update() {
    ship->movement();

    vector3d back_offset = ship->position - ship->direction * 250.0;
    vector3d up_offset = vector3d::up().get_rotated(ship->rotation) * 50.0;
    camera->move(back_offset + up_offset);
    camera->rotate(ship->rotation);
    camera->update();

    for (const auto& a : asteroids)
        a->movement();
}

void object_register::add_asteroid() {
    shared_ptr<asteroid> a = make_shared<asteroid>(
        asset_library::models.at(ASTEROID_MODEL_KEY),
        vector3d{100.0, 100.0, 100.0}, quaternion{}, vector3d{50.0, 50.0, 50.0},
        vector3d::forward(), 10.0
    );

    a->init();
    asteroids.push_back(a);
}