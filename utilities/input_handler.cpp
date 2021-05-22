#include "input_handler.h"
#include "../main/config.h"

bool input::mouse_down = false;
bool input::key_states[]{ false };
int input::mouse_x = 0;
int input::mouse_y = 0;
double input::x_delta = 0.0;
double input::y_delta = 0.0;

void input_handler::on_mouse_click(int button, int state, int x, int y)
{
    input::mouse_down = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
}

void input_handler::on_key_down(unsigned char key, int x, int y)
{
    if (key == 27) exit(EXIT_SUCCESS);
    input::key_states[ANY] = true;

    switch (key)
    {
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
    case 32:
        input::key_states[SPACEBAR] = true;
        break;
    default:
        break;
    }
}

void input_handler::on_key_up(unsigned char key, int x, int y)
{
    input::key_states[ANY] = false;

    switch (key)
    {
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
    case 32:
        input::key_states[SPACEBAR] = false;
        break;
    default:
        break;
    }
}

void input_handler::on_mouse_motion(int x, int y) {
    double centre_x = g::width / 2.0;
    double centre_y = g::height / 2.0;

    input::x_delta = (static_cast<double>(x) - centre_x) / centre_x;
    // printf("x (%.3f - %.3f) / %.3f = %.3f\n", static_cast<double>(x), centre_x, centre_x, input::x_delta);
    input::y_delta = (centre_y - static_cast<double>(y)) / centre_y;
    // printf("y (%.3f - %.3f) / %.3f = %.3f\n", static_cast<double>(y), centre_y, centre_y, input::y_delta);
    input::mouse_x = x;
    input::mouse_y = y;

}
