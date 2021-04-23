#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <vector>

#include "point3d.h"
#include "../main/config.h"

// this namespace contains definitions of static math functions used all throughout
namespace math {
    static double to_radians(double degrees) {
        return (degrees * M_PI) / 180;
    }

    static double to_degrees(double radians) {
        return radians * (180 / M_PI);
    }

    // generate double random numbers
    static double get_random(double lower, double upper) {
        // TODO make use <random> engine and real distribution
        return lower + (rand() / (RAND_MAX / (upper - lower)));
    }

    // linear interpolation for double numbers
    static double lerp(double start, double end, double t) {
        return start + (end - start) * t;
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

    // TODO draw a debug circle
//    static void draw_circle(const double& radius, const int& segments) {
//        glBegin(GL_LINE_LOOP);
//        double circle_angle = 0.0;
//        for (int i = 0; i < segments; ++i) {
//            circle_angle += 360.0 / segments;
//            for (int j = 0; j < segments; ++j) {
//
//                double circle_x = cos(to_radians(circle_angle)) * radius;
//                double circle_y = sin(to_radians(circle_angle)) * radius;
//                glVertex2d(circle_x, circle_y);
//            }
//        }
//        glEnd();
//    }

    // TODO get pre-calculated points for a circle of the provided radius and segments
//    static std::vector<point3d*> get_circle_points(double radius, unsigned int rings, unsigned int sectors)
//    {
//        std::vector<point3d*> points;
//        double circle_angle = 0.0;
//        for (int input = 0; input < segments; ++input)
//        {
//            double circle_x = cos(to_radians(circle_angle)) * radius;
//            double circle_y = sin(to_radians(circle_angle)) * radius;
//            points.push_back(new point3d(circle_x, circle_y));
//            circle_angle += 360.0 / segments;
//        }
//        return points;
//    }
}

#endif // !MATH_H
