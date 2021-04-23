#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

// simple window object with relevant properties
class game_window {
public:
    game_window(const int& position_x, const int& position_y,
        const int& width, const int& height, const int& is_fullscreen);

    int position_x;
    int position_y;
    int width;
    int height;
    bool is_fullscreen;
};

#endif // !GAME_WINDOW_H
