#ifndef DRAW_SPHERE_H
#define DRAW_SPHERE_H

#include "graphics.h"

#include <cmath>
#include <vector>

using std::vector;

//void draw_sphere() {
//    double pi = 3.141592;
//    double di = 0.02;
//    double dj = 0.04;
//    double du = di * 2 * pi;
//    double dv = dj * pi;
//
//    for (double i = 0; i < 1.0;) {
//        for (double j = 0; j < 1.0;) {
//            double u = i * 2 * pi;
//            double v = (j - 0.5) * pi;
//
//            double p[][3] = {
//                cos(v) * cos(u), cos(v) * sin(u), sin(v),
//                cos(v) * cos(u + du), cos(v) * sin(u + du), sin(v),
//                cos(v + dv) * cos(u + du), cos(v + dv) * sin(u + du), sin(v + dv),
//                cos(v + dv) * cos(u), cos(v + dv) * sin(u), sin(v + dv) };
//
//            glBegin(GL_POLYGON);
//            glNormal3dv(p[0]);
//            glTexCoord2d(i, j);
//            glVertex3dv(p[0]);
//            glNormal3dv(p[1]);
//            glTexCoord2d(i + di, j);
//            glVertex3dv(p[1]);
//            glNormal3dv(p[2]);
//            glTexCoord2d(i + di, j + dj);
//            glVertex3dv(p[2]);
//            glNormal3dv(p[3]);
//            glTexCoord2d(i, j + dj);
//            glVertex3dv(p[3]);
//            glEnd();
//
//            j += dj;
//        }
//
//        i += di;
//    }
//}


void draw_sphere(double radius, int sector_count, int stack_count) {
    vector<double> positions;
    vector<double> normals;
    vector<double> texcoords;

    vector<vertex_t> vertices;

    double x, y, z, xy;                                      // vertex position
    double nx, ny, nz, radius_inverse = 1.0 / radius;       // vertex normal
    double s, t;                                             // vertex texCoord

    double sector_step = 2 * M_PI / sector_count;
    double stack_step = M_PI / stack_count;
    double sector_angle, stack_angle;

    for (size_t i = 0; i <= stack_count; ++i) {
        stack_angle = M_PI / 2 - static_cast<double>(i) * stack_step;    // starting from pi/2 to -pi/2
        xy = radius * cos(stack_angle);             // r * cos(u)
        z = radius * sin(stack_angle);              // r * sin(u)

        // add (sectorCount+1) positions per stack
        // the first and last positions have same position and normal, but different texcoords
        for (size_t j = 0; j <= sector_count; ++j) {
            sector_angle = static_cast<double>(j) * sector_step;         // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cos(sector_angle);             // r * cos(u) * cos(v)
            y = xy * sin(sector_angle);             // r * cos(u) * sin(v)
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * radius_inverse;
            ny = y * radius_inverse;
            nz = z * radius_inverse;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex texcoord (s, t) range between [0, 1]
            s = static_cast<double>(j) / sector_count;
            t = static_cast<double>(i) / stack_count;
            texcoords.push_back(s);
            texcoords.push_back(t);

            point3d position{ x, y, z };
            point2d texcoord{ s, t };
            point3d normal{ nx, ny, nz };

            vertices.emplace_back(position, texcoord, normal);
        }
    }

    vector<size_t> indices;
    vector<size_t> line_indices;
    vector<face_t> faces;

    int k1, k2;
    for (size_t i = 0; i < stack_count; ++i) {
        k1 = static_cast<int>(i) * (sector_count + 1);      // beginning of current stack
        k2 = k1 + sector_count + 1;                         // beginning of next stack

        for (size_t j = 0; j < sector_count; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1


            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                faces.emplace_back(array<vertex_t, 3>{
                    vertices.at(k1),
                    vertices.at(k2),
                    vertices.at(k1 + 1)
                });
            }

            // k1+1 => k2 => k2+1
            if (i != (stack_count - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);

                faces.emplace_back(array<vertex_t, 3>{
                    vertices.at(k1 + 1),
                    vertices.at(k2),
                    vertices.at(k2 + 1)
                });
            }


            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            line_indices.push_back(k1);
            line_indices.push_back(k2);
            if (i != 0) {
                // horizontal lines except 1st stack, k1 => k+1
                line_indices.push_back(k1);
                line_indices.push_back(k1 + 1);
            }
        }
    }

    mesh_t mesh{faces};
    mesh.render();
}

#endif // !DRAW_SPHERE_H
