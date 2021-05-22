#include "game_init.h"
#include "config.h"
#include "game_camera.h"
#include "../math/game_math.h"
#include "../utilities/input_handler.h"

#include <array>
#include <memory>
#include <cmath>

using std::array;
using std::shared_ptr;
using std::make_shared;

shared_ptr<vector3d> forward = make_shared<vector3d>(0.0, 0.0, -1.0);

shared_ptr<game_camera> camera;
shared_ptr<vector3d> position = make_shared<vector3d>(0.0, 50.0, 0.0);
shared_ptr<vector3d> direction = make_shared<vector3d>(0.0, 0.0, -1.0);
shared_ptr<quaternion> rotation = make_shared<quaternion>(quaternion::get_identity());

double velocity = 500.0;
double d_angle = 90.0;
double l_time;

shared_ptr<vector3d> x_axis = make_shared<vector3d>(1.0, 0.0, 0.0);
shared_ptr<vector3d> y_axis = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> z_axis = make_shared<vector3d>(0.0, 0.0, 1.0);

void update_axes(const quaternion& q) {
    x_axis->rotate(q);
    y_axis->rotate(q);
    z_axis->rotate(q);
}

void draw_axes() {
    glPushMatrix();

    glLineWidth(4.0);
    glColor3d(color::red.r, color::red.g, color::red.b);
    glBegin(GL_LINES);
    glVertex3d(-500.0, 0.0, 0.0);
    glVertex3d(500.0, 0.0, 0.0);
    glEnd();

    glColor3d(color::green.r, color::green.g, color::green.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, -500.0, 0.0);
    glVertex3d(0.0, 500.0, 0.0);
    glEnd();

    glColor3d(color::blue.r, color::blue.g, color::blue.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -500.0);
    glVertex3d(0.0, 0.0, 500.0);
    glEnd();

    glPopMatrix();
}

void draw_plane() {
    glPushMatrix();

    glColor3d(color::gray.r, color::gray.g, color::gray.b);
    glBegin(GL_QUADS);
    glVertex3d(-500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, 500.0);
    glVertex3d(500.0, 0.0, -500.0);
    glVertex3d(-500.0, 0.0, -500.0);
    glEnd();

    glPopMatrix();
}

void pitch(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            x_axis->x * sin(to_radians(angle * 0.5)),
            x_axis->y * sin(to_radians(angle * 0.5)),
            x_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
}

void roll(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            z_axis->x * sin(to_radians(angle * 0.5)),
            z_axis->y * sin(to_radians(angle * 0.5)),
            z_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
}

void yaw(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            y_axis->x * sin(to_radians(angle * 0.5)),
            y_axis->y * sin(to_radians(angle * 0.5)),
            y_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
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
    draw_plane();

    glPushMatrix();
    glTranslated(position->x, position->y, position->z);
    glMultMatrixd(rotation->to_matrix());
    glScaled(100.0, 100.0, 100.0);

    glColor3d(color::white.r, color::white.g, color::white.b);
    glutWireCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glColor3d(color::purple.r, color::purple.g, color::purple.b);
    glBegin(GL_LINES);
    glVertex3d(position->x, position->y, position->z);

    vector3d target = *position + *direction * 100.0;
    glVertex3d(target.x, target.y, target.z);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
    error_check("game_init::on_display");
}

void movement() {
    if (input::key_states[Q])
        yaw(false, 1.0);
    if (input::key_states[E])
        yaw(true, 1.0);
    if (input::key_states[W])
        pitch(true, 1.0);
    if (input::key_states[S])
        pitch(false, 1.0);
    if (input::key_states[D])
        roll(true, 1.0);
    if (input::key_states[A])
        roll(false, 1.0);
    if (input::key_states[SPACEBAR]) {
        *position += *direction * velocity * g::d_time;
    }

    pitch(input::y_delta < 0.0, abs(input::y_delta));
    yaw(input::x_delta > 0.0, abs(input::x_delta));
    camera->move(*position - *direction * 500.0);
}

void on_reshape(int width, int height) {
    glViewport(ORIGIN_X, ORIGIN_Y, width, height);
    g::width = static_cast<double>(width);
    g::height = static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    camera->look_at({ORIGIN_X, ORIGIN_Y, ORIGIN_Z});

    error_check("game_init::on_reshape");
}

void on_idle() {
    // rotate current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = (g::c_time - l_time) / 1000;
    l_time = g::c_time;

    movement();
    camera->update();
    camera->look_at(*position);
    glutPostRedisplay();
    error_check("game_init::on_idle");
}

// input functions delegate calls to handler class
void on_mouse_click(int button, int state, int x, int y) {
    input_handler::on_mouse_click(button, state, x, y);
    error_check("game_init::on_mouse_click");
}

void on_key_down(unsigned char key, int x, int y) {
    input_handler::on_key_down(key, x, y);
    error_check("game_init::on_key_down");
}

void on_key_up(unsigned char key, int x, int y) {
    input_handler::on_key_up(key, x, y);
    error_check("game_init::on_key_up");
}

void on_mouse_motion(int x, int y) {
    input_handler::on_mouse_motion(x, y);
    error_check("game_init::on_mouse_motion");
}

void init_camera() {
    vector3d camera_position{0.0, 50.0, TOTAL_UNITS / 2.0};
    camera = make_shared<game_camera>(camera_position, quaternion::get_identity());
}


void test_rotation() {
    vector3d v{ 0.0, 1.0, 0.0 };
    // vector3d axis_t{ 0.0, 0.0, 1.0 };
    // v.rotate(90.0, axis_t);

    v.rotate(quaternion::get_identity());
    printf("rotated_v1: %.3f, %.3f, %.3f\n", v.x, v.y, v.z);

    double angle = 90.0;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            x_axis->x * sin(to_radians(angle * 0.5)),
            x_axis->y * sin(to_radians(angle * 0.5)),
            x_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    quaternion q_result = quaternion::get_identity() * qr;

    v.rotate(q_result);
    printf("rotated_v2: %.3f, %.3f, %.3f\n", v.x, v.y, v.z);
    printf("rotated_q: %.3f, %.3f, %.3f, %.3f\n", q_result.w, q_result.x, q_result.y, q_result.z);
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

    glutMouseFunc(on_mouse_click);
    glutPassiveMotionFunc(on_mouse_motion);
    glutIgnoreKeyRepeat(false);
    glutKeyboardFunc(on_key_down);
    glutKeyboardUpFunc(on_key_up);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    init_camera();

    test_rotation();

    glutMainLoop();
}
