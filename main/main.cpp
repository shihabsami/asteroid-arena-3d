#include <cstdlib>
#include <ctime>

#include "game_window.h"
#include "game_init.h"
#include "config.h"

int main(int argc, char** argv) {
    // set the seed for all the random number generations
    srand(static_cast<unsigned int>(time(nullptr)));

    // initialize the game with a window from provided parameters
    auto* main_window = new game_window(
        ORIGIN_X, ORIGIN_Y, INIT_WIDTH, INIT_HEIGHT, IS_FULLSCREEN);
    init_game(&argc, argv, main_window);

    return EXIT_SUCCESS;
}
