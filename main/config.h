#ifndef CONFIG_H
#define CONFIG_H

#define GL_SILENCE_DEPRECATION

#if _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
#endif

#if __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
    #define PATH_SEPARATOR "/"
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#include <iostream>

// initial configurations
#define GAME_TITLE      "Asteroid Arena 3D"
#define INIT_WIDTH      1280
#define INIT_HEIGHT     720
#define IS_FULLSCREEN   true

// the total number of units in the z axis when aspect ratio is 1.0
#define TOTAL_UNITS   1000

// constants used in transformations
#define ORIGIN_X       0
#define ORIGIN_Y       0
#define ORIGIN_Z       0
#define Z_NEAR         1.0
#define Z_FAR          2 * TOTAL_UNITS + Z_NEAR
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


#define CAMERA_LAG 1.0


// g namespace containing all g variables used
namespace g {
    extern double width;
    extern double height;
    extern double d_time;
    extern double c_time;
}

#endif // !CONFIG_H
