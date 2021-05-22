cmake_minimum_required(VERSION 3.20.1)
project(Asteroid_Arena_3D)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WARN_DEPRECATED FALSE)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

include_directories("/usr/include/GL")
set(SOURCE_FILES main/main.cpp
        main/config.cpp main/config.h
        main/game_window.cpp main/game_window.h
        main/game_init.cpp main/game_init.h
        main/camera.cpp main/camera.h
        math/game_math.h
        math/point2d.cpp math/point2d.h
        math/point3d.cpp math/point3d.h
        math/vector3d.cpp math/vector3d.h
        math/quaternion.cpp math/quaternion.h
        utilities/input_handler.cpp utilities/input_handler.h
        utilities/mesh_component.h
        utilities/object_loader.h
        utilities/lighting_component.h utilities/mesh_component.h main/graphics.h
        )

add_executable(Asteroid_Arena_3D ${SOURCE_FILES})
target_link_libraries(Asteroid_Arena_3D ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
