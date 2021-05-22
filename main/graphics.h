#ifndef GRAPHICS_H
#define GRAPHICS_H

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

// checks errors with the help of opengl functions
static void error_check(const char* location) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
        printf("%s: %s\n", location, gluErrorString(error));
}


#endif // !GRAPHICS_H
