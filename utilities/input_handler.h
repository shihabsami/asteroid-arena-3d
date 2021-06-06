#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

namespace input {
    extern bool mouse_states[3];
    extern bool key_states[10];
    extern double x_delta;
    extern double y_delta;
}

enum mouse {
    CLICK, WHEEL_UP, WHEEL_DOWN
};

enum key {
    W, S, A, D, Q, E, C, X, Z, SPACEBAR
};

class input_handler {
public:
    static void on_mouse_click(int button, int state, int x, int y);

    static void on_key_down(unsigned char key, int x, int y);

    static void on_key_up(unsigned char key, int x, int y);

    static void on_mouse_motion(int x, int y);
};

#endif // !INPUT_HANDLER_H
