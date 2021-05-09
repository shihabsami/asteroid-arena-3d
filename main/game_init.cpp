#include "game_init.h"
#include "config.h"
#include "../utilities/input_handler.h"
#include "../math/vector3d.h"
#include "../math/point3d.h"
#include "../math/quaternion.h"

#include <array>
#include <memory>
#include <cmath>

using std::array;
using std::shared_ptr;
using std::make_shared;

double l_time;

shared_ptr<vector3d> camera_position;
shared_ptr<vector3d> camera_up;
shared_ptr<vector3d> quad_position;

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

void draw_axes() {
    glPushMatrix();

    glLineWidth(2.0);
    glColor3d(c::red.r, c::red.g, c::red.b);
    glBegin(GL_LINES);
    glVertex3d(-25.0, 0.0, 0.0);
    glVertex3d(25.0, 0.0, 0.0);
    glEnd();

    glColor3d(c::green.r, c::green.g, c::green.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, -25.0, 0.0);
    glVertex3d(0.0, 25.0, 0.0);
    glEnd();

    glColor3d(c::blue.r, c::blue.g, c::blue.b);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -25.0);
    glVertex3d(0.0, 0.0, 25.0);
    glEnd();

    glPopMatrix();
}

quaternion q = quaternion(1.0, { 0.0, 0.0, 0.0 });

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

    glPushMatrix();

    glMultMatrixd(q.to_matrix());
    glTranslated(quad_position->x, quad_position->y, quad_position->z);
    glScaled(5.0, 5.0, 5.0);

    glColor3d(c::purple.r, c::purple.g, c::purple.b);
    glutWireCube(1.0);

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
    gluLookAt(
        camera_position->x, camera_position->y, camera_position->z,
        ORIGIN_X, ORIGIN_Y, ORIGIN_Z,
        camera_up->x, camera_up->y, camera_up->z);

    error_check("game_init::on_reshape");
}

void update_position() {
    if (input::key_states[Q])
        quad_position->z += 1;
    if (input::key_states[E])
        quad_position->z -= 1;
    if (input::key_states[W])
        quad_position->y += 1;
    if (input::key_states[S])
        quad_position->y -= 1;
    if (input::key_states[D])
        quad_position->x += 1;
    if (input::key_states[A])
        quad_position->x -= 1;
    if (input::key_states[SPACEBAR])
        q = { 1.0, { 0.0, 0.0, 0.0 }};
}

void pitch(bool negative) {
    vector3d axis = { 1.0, 0.0, 0.0 };
    double angle = (negative ? -5.0 : 5.0) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            axis.x * sin(angle * 0.5),
            axis.y * sin(angle * 0.5),
            axis.z * sin(angle * 0.5)
        }
    );

    q = qr * q;
}

void roll(bool negative) {
    vector3d axis = { 0.0, 0.0, 1.0 };
    double angle = (negative ? -5.0 : 5.0) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            axis.x * sin(angle * 0.5),
            axis.y * sin(angle * 0.5),
            axis.z * sin(angle * 0.5)
        }
    );

    q = qr * q;
}

void yaw(bool negative) {
    vector3d axis = { 0.0, 1.0, 0.0 };
    double angle = (negative ? -5.0 : 5.0) * g::d_time;

    quaternion qr = quaternion(
        cos(angle * 0.5), {
            axis.x * sin(angle * 0.5),
            axis.y * sin(angle * 0.5),
            axis.z * sin(angle * 0.5)
        }
    );

    q = qr * q;
}

void on_rotate(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_UP:
        pitch(true);
        break;
    case GLUT_KEY_DOWN:
        pitch(false);
        break;
    case GLUT_KEY_LEFT:
        roll(false);
        break;
    case GLUT_KEY_RIGHT:
        roll(true);
        break;
    case GLUT_KEY_PAGE_UP:
        yaw(true);
        break;
    case GLUT_KEY_PAGE_DOWN:
        yaw(false);
        break;
    default:
        break;
    }

    auto* f = q.to_matrix();
    printf(
        "%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f, %.3f\n%.3f %.3f %.3f %.3f\n%.3f %.3f %.3f %.3f\n\n",
        f[0], f[1], f[2], f[3],
        f[4], f[5], f[6], f[7],
        f[8], f[9], f[10], f[11],
        f[12], f[13], f[14], f[15]);

    printf("q.m:   %f\n", q.get_magnitude());
}

void on_idle() {
    // update current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = (g::c_time - l_time) / 1000;
    l_time = g::c_time;

    update_position();
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

void init_camera() {
    camera_position = make_shared<vector3d>(0.0, 10.0, TOTAL_UNITS / 2.0);
    camera_up = make_shared<vector3d>(0.0, 1.0, 0.0);
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
    glutSpecialFunc(on_rotate);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    init_camera();
    quad_position = make_shared<vector3d>();

    glutMainLoop();
}
