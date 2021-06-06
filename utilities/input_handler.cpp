#include "input_handler.h"
#include "../main/graphics.h"
#include "../main/global.h"

#define GLUT_MOUSE_WHEEL_UP 3
#define GLUT_MOUSE_WHEEL_DOWN 4

bool input::mouse_states[]{ false };
bool input::key_states[]{ false };
double input::x_delta = 0.0;
double input::y_delta = 0.0;

void input_handler::on_mouse_click(int button, int state, int x, int y) {
    input::mouse_states[CLICK] = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
    input::mouse_states[WHEEL_UP] = (button == GLUT_MOUSE_WHEEL_UP);
    input::mouse_states[WHEEL_DOWN] = (button == GLUT_MOUSE_WHEEL_DOWN);
}

void input_handler::on_key_down(unsigned char key, int x, int y) {
    if (key == 27) exit(EXIT_SUCCESS);

    switch (key) {
    case 'w':
    case 'W':
        input::key_states[W] = true;
        break;
    case 's':
    case 'S':
        input::key_states[S] = true;
        break;
    case 'a':
    case 'A':
        input::key_states[A] = true;
        break;
    case 'd':
    case 'D':
        input::key_states[D] = true;
        break;
    case 'q':
    case 'Q':
        input::key_states[Q] = true;
        break;
    case 'e':
    case 'E':
        input::key_states[E] = true;
        break;
    case 'c':
    case 'C':
        input::key_states[C] = true;
        break;
    case 'x':
    case 'X':
        input::key_states[X] = true;
        break;
    case 'z':
    case 'Z':
        input::key_states[Z] = true;
        break;
    case 32:
        input::key_states[SPACEBAR] = true;
        break;
    default:
        break;
    }
}

void input_handler::on_key_up(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
    case 'W':
        input::key_states[W] = false;
        break;
    case 's':
    case 'S':
        input::key_states[S] = false;
        break;
    case 'a':
    case 'A':
        input::key_states[A] = false;
        break;
    case 'd':
    case 'D':
        input::key_states[D] = false;
        break;
    case 'q':
    case 'Q':
        input::key_states[Q] = false;
        break;
    case 'e':
    case 'E':
        input::key_states[E] = false;
        break;
    case 'c':
    case 'C':
        input::key_states[C] = false;
        break;
    case 'x':
    case 'X':
        input::key_states[X] = false;
        break;
    case 'z':
    case 'Z':
        input::key_states[Z] = false;
        break;
    case 32:
        input::key_states[SPACEBAR] = false;
        break;
    default:
        break;
    }
}

void input_handler::on_mouse_motion(int x, int y) {
    double centre_x = global::width / 2.0;
    double centre_y = global::height / 2.0;

    input::x_delta = 0.0;
    input::y_delta = 0.0;
    if (x > centre_x - 50 && x < centre_x + 50 && y > centre_y - 50 && y < centre_x + 50) return;

    input::x_delta = (centre_x - static_cast<double>(x)) / centre_x;
    input::y_delta = (centre_y - static_cast<double>(y)) / centre_y;
}
