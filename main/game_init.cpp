#include "game_init.h"
#include "config.h"
#include "../math/point3d.h"
#include "../math/vector3d.h"
#include "../math/quaternion.h"
#include "../utilities/input_handler.h"

#include <array>
#include <memory>
#include <cmath>

using std::array;
using std::shared_ptr;
using std::make_shared;

struct cube {
    shared_ptr<array<point3d, 8>> vertices;
    shared_ptr<array<point3d, 8>> normals;
    shared_ptr<array<array<point3d, 3>, 12>> faces;

    void draw() {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glBegin(GL_TRIANGLES);

        for (auto& face : *faces) {
            for (int j = 0; j < face.size(); ++j) {
                glNormal3d(normals->at(j).x, normals->at(j).y, normals->at(j).z);
                glVertex3d(face.at(j).x, face.at(j).y, face.at(j).z);
            }
        }

        glEnd();

        glPopMatrix();
    }
};

shared_ptr<vector3d> camera_position = make_shared<vector3d>(0.0, 0.0, TOTAL_UNITS / 2.0);
shared_ptr<vector3d> camera_up = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> cube_position = make_shared<vector3d>(0.0, 0.0, 0.0);
shared_ptr<vector3d> direction = make_shared<vector3d>(0.0, 0.0, -1.0);
shared_ptr<quaternion> rotation = make_shared<quaternion>(1.0, 0.0, 0.0, 0.0);
double velocity = 200.0;
double d_angle = 2.0;
double l_time;

shared_ptr<vector3d> x_axis = make_shared<vector3d>(1.0, 0.0, 0.0);
shared_ptr<vector3d> y_axis = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> z_axis = make_shared<vector3d>(0.0, 0.0, 1.0);

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
    update_axes(qr);
}

void test_rotation() {
    vector3d v_t{ 1.0, 0.0, 0.0 };
    // quaternion q_t{ 0.707, 0.0, 0.0, 0.707 };
    vector3d axis_t{ 0.0, 0.0, 1.0 };
    v_t.rotate(90.0, axis_t);

    printf("%.3f, %.3f, %.3f\n", v_t.x, v_t.y, v_t.z);
}

void draw_axes() {
    glPushMatrix();

    glLineWidth(2.0);
    glColor3d(c::red.r, c::red.g, c::red.b);
    glBegin(GL_LINES);
    glVertex3d(-500.0, 0.0, 0.0);
    glVertex3d(500.0, 0.0, 0.0);
    glEnd();

    glColor3d(c::green.r, c::green.g, c::green.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, -500.0, 0.0);
    glVertex3d(0.0, 500.0, 0.0);
    glEnd();

    glColor3d(c::blue.r, c::blue.g, c::blue.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -500.0);
    glVertex3d(0.0, 0.0, 500.0);
    glEnd();

    glPopMatrix();
}

void draw_plane() {
    glPushMatrix();

    glColor3d(c::gray.r, c::gray.g, c::gray.b);
    glBegin(GL_QUADS);
    glVertex3d(-500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, -500.0);
    glVertex3d(-500.0, 0.0, -500.0);
    glEnd();

    glPopMatrix();
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor
        (
            static_cast<GLclampf>(c::black.r),
            static_cast<GLclampf>(c::black.g),
            static_cast<GLclampf>(c::black.b), 1.0
        );

    glMatrixMode(GL_MODELVIEW);
    draw_axes();
    draw_plane();

    glPushMatrix();
    glTranslated(cube_position->x, cube_position->y, cube_position->z);
    glMultMatrixd(rotation->to_matrix());
    glScaled(50.0, 50.0, 50.0);
    glColor3d(c::purple.r, c::purple.g, c::purple.b);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glScaled(100.0, 100.0, 100.0);
    glColor3d(c::white.r, c::white.g, c::white.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(direction->x, direction->y, direction->z);
    glEnd();
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

    *camera_position = *cube_position - *direction * 250.0;

//    auto* f = rotation->to_matrix();
//    printf(
//        "%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f, %.3f\n%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f %.3f\n\n",
//        f[0], f[1], f[2], f[3],
//        f[4], f[5], f[6], f[7],
//        f[8], f[9], f[10], f[11],
//        f[12], f[13], f[14], f[15]);
//
//    printf("direction.x: %3f, direction.y: %.3f, direction.z: %.3f\n", direction->x, direction->y, direction->z);
//
//    printf("rotation.n: %f\n", rotation->get_norm());
//    printf("direction.m: %f\n", direction->get_magnitude());
}

void update_camera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera_position->x, camera_position->y, camera_position->z,
        cube_position->x, cube_position->y, cube_position->z,
        camera_up->x, camera_up->y, camera_up->z);
}

void on_idle() {
    // update current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = (g::c_time - l_time) / 1000;
    l_time = g::c_time;

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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutMainLoop();
}
