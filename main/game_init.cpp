#include "game_init.h"
#include "config.h"
#include "../utilities/input_handler.h"

double l_time;

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
    glLoadIdentity();
    draw_axes();

    glPushMatrix();
    glScaled(10.0, 10.0, 10.0);

    glColor3d(c::purple.r, c::purple.g, c::purple.b);
    glBegin(GL_POLYGON);
    glVertex3d(-1.0, -1.0, 0.0);
    glVertex3d(1.0, -1.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(-1.0, 1.0, 0.0);
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
    gluLookAt(
        0.0, 0.0, TOTAL_UNITS,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    error_check("game_init::on_reshape");
}

void on_idle() {
    // update current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = g::c_time - l_time;
    l_time = g::c_time;
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
    glutMainLoop();
}
