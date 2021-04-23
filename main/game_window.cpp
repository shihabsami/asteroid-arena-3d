#include "game_window.h"

game_window::game_window(const int& position_x, const int& position_y,
    const int& width, const int& height, const int& is_fullscreen)
    : position_x(position_x), position_y(position_y),
    width(width), height(height), is_fullscreen(is_fullscreen) {}
