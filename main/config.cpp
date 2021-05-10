#include "config.h"

// this file only contains definition of global variables
double g::width;
double g::height;
double g::d_time;
double g::c_time;

color_t c::white{ 1.0, 1.0, 1.0 };
color_t c::black{ 0.0, 0.0, 0.0 };
color_t c::gray{0.25, 0.25, 0.25};
color_t c::red{ 1.0, 0.0, 0.0 };
color_t c::green{ 0.0, 1.0, 0.0 };
color_t c::blue{ 0.0, 0.0, 1.0 };
color_t c::purple{ 1.0, 0.0, 1.0 };

// checks errors with the help of opengl functions
void error_check(const char* location) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
        printf("%s: %s\n", location, gluErrorString(error));
}
