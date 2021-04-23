#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

namespace input
{
    extern bool mouse_down;
    extern bool key_states[8];
}

enum key
{
    W, S, A, D, Q, E, SPACEBAR, ANY
};

class input_handler
{
public:
    static void on_mouseclick(int button, int state, int x, int y);

    static void on_keydown(unsigned char key, int x, int y);

    static void on_keyup(unsigned char key, int x, int y);
};

#endif // !INPUT_HANDLER_H
