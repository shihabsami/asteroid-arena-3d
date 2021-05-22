#include "game_init.h"
#include "config.h"
#include "../math/point3d.h"
#include "../math/vector3d.h"
#include "../math/quaternion.h"
#include "../utilities/input_handler.h"
#include "../utilities/object_loader.h"
#include "../utilities/lighting_component.h"
#include "camera.h"

#include <array>
#include <memory>
#include <cmath>

using std::array;
using std::shared_ptr;
using std::make_shared;

shared_ptr<vector3d> camera_position = make_shared<vector3d>(0.0, 15.0, TOTAL_UNITS / 2.0);
shared_ptr<vector3d> camera_up = make_shared<vector3d>(0.0, 1.0, 0.0);
camera cam{{ 0.0, 15.0, TOTAL_UNITS / 2.0 }, { 0.0, 1.0, 0.0 }};

shared_ptr<vector3d> cube_position = make_shared<vector3d>(0.0, 0.0, 0.0);
shared_ptr<vector3d> direction = make_shared<vector3d>(0.0, 0.0, -1.0);
shared_ptr<quaternion> rotation = make_shared<quaternion>(quaternion::get_identity());
double velocity = 500.0;
double d_angle = 2.0;
double l_time;

shared_ptr<vector3d> x_axis = make_shared<vector3d>(1.0, 0.0, 0.0);
shared_ptr<vector3d> y_axis = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> z_axis = make_shared<vector3d>(0.0, 0.0, 1.0);

void test_rotation() {
    vector3d v_t{ 1.0, 0.0, 0.0 };
    vector3d axis_t{ 0.0, 0.0, 1.0 };
    // v_t.rotate(90.0, axis_t);

    quaternion q{ 1.0, 0.0, 0.0, 0.0 };
    v_t.rotate(q);

    printf("%.3f, %.3f, %.3f\n", v_t.x, v_t.y, v_t.z);
}

void test_quaternion() {
    quaternion q_x{ 0.707, 0.707, 0.0, 0.0 };
    quaternion q_z{ 0.707, 0.0, 0.0, 0.707 };

    quaternion q_result = quaternion::slerp(q_x, q_z, 0.5);

    printf("%.3f, %.3f, %.3f, %.3f\n", q_result.w, q_result.x, q_result.y, q_result.z);
}

shared_ptr<mesh_t> test_obj(const char* filename) {
    stringstream stream;
    stream << ".." << PATH_SEPARATOR << "files" << PATH_SEPARATOR << "models" << PATH_SEPARATOR << filename;
    string filename_str = stream.str();
    return load_obj(filename_str.c_str());
}

shared_ptr<mesh_t> body = test_obj("helicopter\\body.obj");
shared_ptr<mesh_t> blades = test_obj("helicopter\\blades.obj");
shared_ptr<mesh_t> tail = test_obj("helicopter\\tail.obj");
double blade_rotation = 0.0;

void update_axes(const quaternion& q) {
    x_axis->rotate(q);
    y_axis->rotate(q);
    z_axis->rotate(q);
}

void pitch(bool negative) {
    double angle = (negative ? -d_angle : d_angle) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            x_axis->x * sin(angle * 0.5),
            x_axis->y * sin(angle * 0.5),
            x_axis->z * sin(angle * 0.5)
        }
    );

    *rotation = qr * *rotation;
    direction->rotate(qr);

    camera_up->rotate(qr);
    // cam.rotate(qr);

    update_axes(qr);
}

void roll(bool negative) {
    double angle = (negative ? -d_angle : d_angle) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            z_axis->x * sin(angle * 0.5),
            z_axis->y * sin(angle * 0.5),
            z_axis->z * sin(angle * 0.5)
        }
    );

    *rotation = qr * *rotation;
    direction->rotate(qr);

    camera_up->rotate(qr);
    // cam.rotate(qr);

    update_axes(qr);
}

void yaw(bool negative) {
    double angle = (negative ? -d_angle : d_angle) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            y_axis->x * sin(angle * 0.5),
            y_axis->y * sin(angle * 0.5),
            y_axis->z * sin(angle * 0.5)
        }
    );

    *rotation = qr * *rotation;
    direction->rotate(qr);

    camera_up->rotate(qr);
    // cam.rotate(qr);

    update_axes(qr);
}

void draw_axes() {
    glPushMatrix();

    glLineWidth(4.0);
    // glColor3d(c::red.r, c::red.g, c::red.b);
    set_material(material::red);
    glBegin(GL_LINES);
    glVertex3d(-500.0, 0.0, 0.0);
    glVertex3d(500.0, 0.0, 0.0);
    glEnd();

    // glColor3d(c::green.r, c::green.g, c::green.b);
    set_material(material::green);
    glBegin(GL_LINES);
    glVertex3d(0.0, -500.0, 0.0);
    glVertex3d(0.0, 500.0, 0.0);
    glEnd();

    // glColor3d(c::blue.r, c::blue.g, c::blue.b);
    set_material(material::blue);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -500.0);
    glVertex3d(0.0, 0.0, 500.0);
    glEnd();

    glPopMatrix();
}

void draw_plane() {
    glPushMatrix();

    // glColor3d(c::gray.r, c::gray.g, c::gray.b);
    set_material(material::white);
    glBegin(GL_QUADS);
    glVertex3d(-500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, -500.0);
    glVertex3d(-500.0, 0.0, -500.0);
    glEnd();

    glPopMatrix();
}

shared_ptr<mesh_t> wall = test_obj("wall.obj");

void draw_arena() {
    glPushMatrix();
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0, -1.0, 0.0, 0.0);
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0, 1.0, 0.0, 0.0);
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0, 0.0,0.0, 1.0);
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0, 0.0, 0.0, -1.0);
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glRotated(180.0, 0.0, 0.0, 0.0);
    glScaled(500.0, 500.0, 500.0);
    set_material(material::purple);
    for (const auto& f : wall->faces) {
        glBegin(GL_LINE_LOOP);
        glVertex3d(f.vertices[0].position.x, f.vertices[0].position.y, f.vertices[0].position.z);
        glVertex3d(f.vertices[1].position.x, f.vertices[1].position.y, f.vertices[1].position.z);
        glVertex3d(f.vertices[2].position.x, f.vertices[2].position.y, f.vertices[2].position.z);
        glEnd();
    }
    glPopMatrix();
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor
        (
            static_cast<GLclampf>(color::black.r),
            static_cast<GLclampf>(color::black.g),
            static_cast<GLclampf>(color::black.b), 1.0
        );

    glMatrixMode(GL_MODELVIEW);
    draw_axes();
    // draw_plane();
    draw_arena();

    glPushMatrix();
    glTranslated(cube_position->x, cube_position->y, cube_position->z);
    glMultMatrixd(rotation->to_matrix());
    glRotated(0.0, 0.0, 1.0, 0.0);
    glScaled(250.0, 250.0, 250.0);

    GLfloat array[3]{static_cast<GLfloat>(cube_position->x), static_cast<GLfloat>(cube_position->y), static_cast<GLfloat>(cube_position->z)};
    glLightfv(GL_LIGHT1, GL_POSITION, array);

    set_material(material::cube_material);
    glPushMatrix();
    glRotated(blade_rotation, 0.0, 1.0, 0.0);
    blades->render();
    glPopMatrix();

    set_material(material::model_material);
    body->render();
    tail->render();
    glPopMatrix();

    glutSwapBuffers();
    error_check("game_init::on_display");
}

void on_reshape(int width, int height) {
    glViewport(ORIGIN_X, ORIGIN_Y, width, height);
    g::width = static_cast<double>(width);
    g::height = static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera_position->x, camera_position->y, camera_position->z,
        ORIGIN_X, ORIGIN_Y, ORIGIN_Z,
        camera_up->x, camera_up->y, camera_up->z);

    // cam.look_at({ORIGIN_X, ORIGIN_Y, ORIGIN_Z});


    error_check("game_init::on_reshape");
}

void update_position() {
    if (input::key_states[Q])
        yaw(false);
    if (input::key_states[E])
        yaw(true);
    if (input::key_states[W])
        pitch(true);
    if (input::key_states[S])
        pitch(false);
    if (input::key_states[D])
        roll(true);
    if (input::key_states[A])
        roll(false);
    if (input::key_states[SPACEBAR]) {
        *cube_position += *direction * velocity * g::d_time;
    }

    *camera_position = *cube_position - *direction * 500.0;
    camera_position->y += 50.0;

    // cam.position = *cube_position - *direction * 500.0;
    // cam.position.y += 50.0;
}

void update_camera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera_position->x, camera_position->y, camera_position->z,
        cube_position->x, cube_position->y, cube_position->z,
        camera_up->x, camera_up->y, camera_up->z);

    // cam.look_at(*cube_position);
}

void on_idle() {
    // rotate current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = (g::c_time - l_time) / 1000;
    l_time = g::c_time;

    // blade_rotation = fmod(blade_rotation + 1080.0 * g::d_time, 360.0);

    update_position();
    update_camera();
    glutPostRedisplay();
    error_check("game_init::on_idle");
}

// input functions delegate calls to handler class
void on_mouseclick(int button, int state, int x, int y) {
    input_handler::on_mouseclick(button, state, x, y);
    error_check("game_init::on_mouseclick");
}

void on_keydown(unsigned char key, int x, int y) {
    input_handler::on_keydown(key, x, y);
    error_check("game_init::on_keydown");
}

void on_keyup(unsigned char key, int x, int y) {
    input_handler::on_keyup(key, x, y);
    error_check("game_init::on_keyup");
}

void init_lighting() {
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glPushMatrix();

    GLfloat light0_position[4]{ 0.0, 10.0, 0.0, 1.0 };
    set_light(GL_LIGHT0, material::light0_material, light0_position);
    GLfloat light1_position[4]{ 0.0, 10.0, 0.0, 1.0 };
    set_light(GL_LIGHT1, material::light0_material, light1_position);


    // glPopMatrix();
}

void init_game(int* argcp, char** argv, game_window* window) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(window->position_x, window->position_y);
    glutInitWindowSize(window->width, window->height);

    g::width = static_cast<double>(window->width);
    g::height = static_cast<double>(window->height);

    glutCreateWindow(GAME_TITLE);
    // if (window->is_fullscreen) glutFullScreen();

    // register callback methods
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutIdleFunc(on_idle);

    glutMouseFunc(on_mouseclick);
    glutIgnoreKeyRepeat(false);
    glutKeyboardFunc(on_keydown);
    glutKeyboardUpFunc(on_keyup);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    init_lighting();

    // test_rotation();

    glutMainLoop();
}
