#include "input_handler.h"
#include "../main/config.h"

bool input::mouse_down = false;
bool input::key_states[]{ false };

void input_handler::on_mouseclick(int button, int state, int x, int y)
{
    input::mouse_down = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
}

void input_handler::on_keydown(unsigned char key, int x, int y)
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

void input_handler::on_keyup(unsigned char key, int x, int y)
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
