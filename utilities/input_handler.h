#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

namespace input
{
    extern bool mouse_down;
    extern bool key_states[8];
    extern int mouse_x;
    extern int mouse_y;
    extern double x_delta;
    extern double y_delta;
}

enum key
{
    W, S, A, D, Q, E, SPACEBAR, ANY
};

class input_handler
{
public:
    static void on_mouse_click(int button, int state, int x, int y);

    static void on_key_down(unsigned char key, int x, int y);

    static void on_key_up(unsigned char key, int x, int y);

    static void on_mouse_motion(int x, int y);
};

#endif // !INPUT_HANDLER_H
