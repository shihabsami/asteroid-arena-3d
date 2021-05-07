#ifndef CONFIG_H
#define CONFIG_H

#if _WIN32
    #include <windows.h>
#endif

#if __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#include <memory>

#endif

// initial configurations
#define GAME_TITLE "Asteroid Arena 3D"
#define INIT_WIDTH      1280
#define INIT_HEIGHT     720
#define IS_FULLSCREEN   true

// the total number of units in the z axis when aspect ratio is 1.0
#define TOTAL_UNITS   50
#define MAX_ANGLE   360.0

// constants used in transformations
#define ORIGIN_X       0
#define ORIGIN_Y       0
#define Z_NEAR         1.0
#define Z_FAR          TOTAL_UNITS + Z_NEAR
#define FOV            45.0

/*
 * arena width should ideally be within TOTAL_UNITS else it shall exceed
 * the coordinate system's maximum size and shall not be fully visible
 */
#define ARENA_WIDTH                800
#define ARENA_HEIGHT               800
#define ARENA_LENGTH               800
#define ARENA_WALL_WIDTH           4.0
#define WALL_COLLISION_THRESHOLD   100

// global namespace containing all global variables used
namespace g {
    extern double width;
    extern double height;
    extern double x_units;
    extern double y_units;
    extern double d_time;
    extern double c_time;
    extern double arena_x_min;
    extern double arena_x_max;
    extern double arena_y_min;
    extern double arena_y_max;
    extern double arena_z_min;
    extern double arena_z_max;
}

struct color_t {
    GLdouble r, g, b;
};

// color namespace, containing all the colors used
namespace c {
    extern std::shared_ptr<color_t> white;
    extern color_t black;
    extern color_t red;
    extern color_t green;
    extern color_t blue;
    extern color_t purple;
}

// function to check errors
extern void error_check(const char* location);

#endif // !CONFIG_H
