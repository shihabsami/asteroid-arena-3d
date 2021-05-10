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

#endif // !GRAPHICS_H
