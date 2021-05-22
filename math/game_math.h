#ifndef MATH_H
#define MATH_H

#include <cmath>

#include "../utilities/lighting_component.h"

using std::lerp;

// this namespace contains definitions of static math functions used all throughout
static double to_radians(double degrees) {
    return (degrees * M_PI) / 180;
}

static double to_degrees(double radians) {
    return radians * (180 / M_PI);
}

// generate double random numbers
//static double get_random(double lower, double upper) {
//    // TODO make use <random> engine and real distribution
//    return lower + (rand() / (RAND_MAX / (upper - lower)));
//}


// linear interpolation for color_t values
static color_t lerp(color_t start, color_t end, double t) {
    return
        {
            lerp(start.r, end.r, t),
            lerp(start.g, end.g, t),
            lerp(start.b, end.b, t)
        };
}

#endif // !MATH_H
