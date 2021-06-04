#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <limits>
#include <random>

#include "../utilities/lighting_component.h"

using std::lerp;
using std::random_device;
using std::uniform_real_distribution;

// this namespace contains definitions of static math functions used all throughout
static double to_radians(double degrees) {
    return (degrees * M_PI) / 180;
}

static double to_degrees(double radians) {
    return radians * (180 / M_PI);
}

// generate double random numbers
static double get_random(double lower, double upper) {
    random_device engine;
    uniform_real_distribution distribution(lower, upper);
    return distribution(engine);
}

// linear interpolation for color_t values
static color_t lerp(color_t start, color_t end, double t) {
    return
        {
            lerp(start.r, end.r, t),
            lerp(start.g, end.g, t),
            lerp(start.b, end.b, t)
        };
}

// linear interpolation for material_t values
static material_t lerp(material_t start, material_t end, double t) {
    return
        {
            {
                lerp(start.ambient[0], end.ambient[0], (float) t),
                lerp(start.ambient[1], end.ambient[1], (float) t),
                lerp(start.ambient[2], end.ambient[2], (float) t),
                lerp(start.ambient[3], end.ambient[3], (float) t) },
            {
                lerp(start.diffuse[0], end.diffuse[0], (float) t),
                lerp(start.diffuse[1], end.diffuse[1], (float) t),
                lerp(start.diffuse[2], end.diffuse[2], (float) t),
                lerp(start.diffuse[3], end.diffuse[3], (float) t) },
            {
                lerp(start.specular[0], end.specular[0], (float) t),
                lerp(start.specular[1], end.specular[1], (float) t),
                lerp(start.specular[2], end.specular[2], (float) t),
                lerp(start.specular[3], end.specular[3], (float) t) },
            {
                lerp(start.emission[0], end.emission[0], (float) t),
                lerp(start.emission[1], end.emission[1], (float) t),
                lerp(start.emission[2], end.emission[2], (float) t),
                lerp(start.emission[3], end.emission[3], (float) t) },
            lerp(start.shininess, end.shininess, (float) t)
        };
}

#endif // !MATH_H
