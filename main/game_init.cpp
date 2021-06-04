#include "game_init.h"
#include "config.h"
#include "global.h"
#include "../utilities/asset_library.h"
#include "../utilities/game_manager.h"
#include "../utilities/input_handler.h"
#include "../utilities/object_register.h"
#include "../utilities/collision_handler.h"

double l_time;
shared_ptr<game_manager> manager;

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    manager->display();

    glutSwapBuffers();
    error_check("game_init::on_display");
}

void on_reshape(int width, int height) {
    glViewport(ORIGIN_X, ORIGIN_Y, width, height);
    global::width = static_cast<double>(width);
    global::height = static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, global::width / global::height, Z_NEAR, Z_FAR);

    error_check("game_init::on_reshape");
}

void on_idle() {
    // rotate current and delta time
    global::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    global::d_time = (global::c_time - l_time) / 1000;
    l_time = global::c_time;
    manager->update();

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

void init_game(int* argcp, char** argv, game_window* window) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(window->position_x, window->position_y);
    glutInitWindowSize(window->width, window->height);

    global::width = static_cast<double>(window->width);
    global::height = static_cast<double>(window->height);

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
    gluPerspective(FOV, global::width / global::height, Z_NEAR, Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // enable texture attributes
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // enable lighting attributes
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glShadeModel(GL_SMOOTH);

    asset_library::load_textures();
    asset_library::load_models();

    manager = make_shared<game_manager>();
    manager->o_register = make_shared<object_register>();
    manager->c_handler = make_shared<collision_handler>(manager);

    manager->start_game();

    glutMainLoop();
}
