#ifndef ICO_H
#define ICO_H

#include "../utilities/mesh_component.h"
#include "../math/game_math.h"
#include "../math/vector3d.h"

#include <vector>
#include <cmath>

using std::vector;

#define INV_PI (1 / M_PI)
#define INV_PI_2 (1 / M_PI * 0.5)


struct triangle_indices {
    size_t v1;
    size_t v2;
    size_t v3;
};

size_t get_middle_point(size_t p1, size_t p2, vector<vector3d>& vertices, double radius) {
    vector3d point1 = vertices[p1];
    vector3d point2 = vertices[p2];
    vector3d middle{
        (point1.x + point2.x) / 2.0,
        (point1.y + point2.y) / 2.0,
        (point1.z + point2.z) / 2.0
    };

    size_t i = vertices.size();
    vertices.push_back(middle.get_normalised() * radius);
    return i;
}

void draw_ico(double radius, unsigned int precision) {
    vector<vector3d> vertices;
    double t = (1.0 + sqrt(5.0)) / 2.0;

    vertices.emplace_back(vector3d{ -1.0, t, 0.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ 1.0, t, 0.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ -1.0, -t, 0.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ 1.0, -t, 0.0 }.get_normalised() * radius);

    vertices.emplace_back(vector3d{ 0.0, -1.0, t }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ 0.0, 1.0, t }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ 0.0, -1.0, -t }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ 0.0, 1.0, -t }.get_normalised() * radius);

    vertices.emplace_back(vector3d{ t, 0.0, -1.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ t, 0.0, 1.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ -t, 0.0, -1.0 }.get_normalised() * radius);
    vertices.emplace_back(vector3d{ -t, 0.0, 1.0 }.get_normalised() * radius);

    vector<triangle_indices> triangles;

    triangles.emplace_back(triangle_indices{ 0, 11, 5 });
    triangles.emplace_back(triangle_indices{ 0, 5, 1 });
    triangles.emplace_back(triangle_indices{ 0, 1, 7 });
    triangles.emplace_back(triangle_indices{ 0, 7, 10 });
    triangles.emplace_back(triangle_indices{ 0, 10, 11 });

    triangles.emplace_back(triangle_indices{ 1, 5, 9 });
    triangles.emplace_back(triangle_indices{ 5, 11, 4 });
    triangles.emplace_back(triangle_indices{ 11, 10, 2 });
    triangles.emplace_back(triangle_indices{ 10, 7, 6 });
    triangles.emplace_back(triangle_indices{ 7, 1, 8 });

    triangles.emplace_back(triangle_indices{ 3, 9, 4 });
    triangles.emplace_back(triangle_indices{ 3, 4, 2 });
    triangles.emplace_back(triangle_indices{ 3, 2, 6 });
    triangles.emplace_back(triangle_indices{ 3, 6, 8 });
    triangles.emplace_back(triangle_indices{ 3, 8, 9 });

    triangles.emplace_back(triangle_indices{ 4, 9, 5 });
    triangles.emplace_back(triangle_indices{ 2, 4, 11 });
    triangles.emplace_back(triangle_indices{ 6, 2, 10 });
    triangles.emplace_back(triangle_indices{ 8, 6, 7 });
    triangles.emplace_back(triangle_indices{ 9, 8, 1 });

    for (unsigned int i = 0; i < precision; ++i) {
        vector<triangle_indices> refined_triangles;

        for (auto triangle : triangles) {
            size_t a = get_middle_point(triangle.v1, triangle.v2, vertices, radius);
            size_t b = get_middle_point(triangle.v2, triangle.v3, vertices, radius);
            size_t c = get_middle_point(triangle.v3, triangle.v1, vertices, radius);

            refined_triangles.emplace_back(triangle_indices{ triangle.v1, a, c });
            refined_triangles.emplace_back(triangle_indices{ triangle.v2, b, a });
            refined_triangles.emplace_back(triangle_indices{ triangle.v3, c, b });
            refined_triangles.emplace_back(triangle_indices{ a, b, c });
        }

        triangles = move(refined_triangles);
    }

    vector<point2d> texcoords;
//    texcoords.resize(vertices.size());

//    for (size_t i = 0; i < vertices.size(); ++i) {
//        vector3d n = vertices.at(i).get_normalised();
//        texcoords.at(i) = {
//            (atan2(n.x, n.z) + M_PI) * INV_PI_2,
//            (acos(n.y) + M_PI) * INV_PI - 1
//        };
//    }

    for (auto& v : vertices) {
        vector3d n = v.get_normalised();
        texcoords.emplace_back(
                asin(n.x) / M_PI + 0.5,
                asin(n.y) / M_PI + 0.5

            );
    }

    mesh_t mesh;
    mesh.faces.resize(triangles.size());

    for (size_t i = 0; i < triangles.size(); ++i) {
        face_t& face = mesh.faces.at(i);

        vector3d v1_position = vertices.at(triangles.at(i).v1);
        point2d v1_texcoord = texcoords.at(triangles.at(i).v1);
        vector3d v1_normal = vertices.at(triangles.at(i).v1).get_normalised();
        face.vertices.at(0) = {
            { v1_position.x, v1_position.y, v1_position.z },
            { v1_texcoord.x, v1_texcoord.y },
            { v1_normal.x, v1_normal.y, v1_normal.z }
        };

        vector3d v2_position = vertices.at(triangles.at(i).v2);
        point2d v2_texcoord = texcoords.at(triangles.at(i).v2);
        vector3d v2_normal = vertices.at(triangles.at(i).v2).get_normalised();
        face.vertices.at(1) = {
            { v2_position.x, v2_position.y, v2_position.z },
            { v2_texcoord.x, v2_texcoord.y },
            { v2_normal.x, v2_normal.y, v2_normal.z }
        };

        vector3d v3_position = vertices.at(triangles.at(i).v3);
        point2d v3_texcoord = texcoords.at(triangles.at(i).v3);
        vector3d v3_normal = vertices.at(triangles.at(i).v3).get_normalised();
        face.vertices.at(2) = {
            { v3_position.x, v3_position.y, v3_position.z },
            { v3_texcoord.x, v3_texcoord.y },
            { v3_normal.x, v3_normal.y, v3_normal.z }
        };
    }



//    for (auto& face : mesh.faces) {
//        vector3d ca, cb, cp;
//        vertex_t& v0 = face.vertices.at(0);
//        vertex_t& v1 = face.vertices.at(1);
//        vertex_t& v2 = face.vertices.at(2);
//
//        ca.x = v1.texcoord.x - v0.texcoord.x;
//        ca.y = v1.texcoord.y - v0.texcoord.y;
//
//        cb.x = v2.texcoord.x - v0.texcoord.x;
//        cb.y = v2.texcoord.y - v0.texcoord.y;
//        ca.z = cb.z = 0.0;
//
//        cp = ca.cross(cb);
//
//        if (cp.z <= 0) continue;
//
//        for (auto& vertex : face.vertices)
//            if (vertex.texcoord.x < 0.25)
//                vertex.texcoord.x += 1.0;
//    }

//    double y_min = 0.5, y_max = -0.5;
//    size_t n_pole_i, n_pole_j, s_pole_i, s_pole_j;
//    for (size_t i = 0; i < mesh.faces.size(); ++i) {
//        for (size_t j = 0; j < mesh.faces.at(i).vertices.size(); ++j) {
//            vertex_t& v = mesh.faces.at(i).vertices.at(j);
//            if (v.texcoord.y > y_max) {
//                y_max = v.texcoord.y;
//                n_pole_i = i;
//                n_pole_j = j;
//            }
//            if (v.texcoord.y < y_min) {
//                y_min = v.texcoord.y;
//                s_pole_i = i;
//                s_pole_j = j;
//            }
//        }
//    }
//
//    for (size_t i = 0; i < mesh.faces.size(); ++i) {
//        for (size_t j = 0; j < mesh.faces.at(i).vertices.size(); ++j) {
//            if (i == n_pole_i && j == n_pole_j) {
//                vertex_t max = mesh.faces.at(i).vertices.at(j);
//                mesh.faces.at(i).vertices.at(1) = mesh.faces.at(i).vertices.at(2);
//                mesh.faces.at(i).vertices.at(2) = mesh.faces.at(i).vertices.at(0);
//                mesh.faces.at(i).vertices.at(0) = max;
//            }
//            if (i == s_pole_i && j == s_pole_j) {
//                vertex_t min = mesh.faces.at(i).vertices.at(j);
//                mesh.faces.at(i).vertices.at(2) = mesh.faces.at(i).vertices.at(1);
//                mesh.faces.at(i).vertices.at(1) = mesh.faces.at(i).vertices.at(0);
//                mesh.faces.at(i).vertices.at(0) = min;
//            }
//        }
//    }
//
//    face_t& n_tri = mesh.faces.at(n_pole_i);
//    n_tri.vertices.at(0).texcoord.x = (n_tri.vertices.at(1).texcoord.x +
//        n_tri.vertices.at(2).texcoord.x) * 0.5;

//    set_material(material::white);
//    glPointSize(20.0);
//    glBegin(GL_POINTS);
//    glVertex3d(min.position.x, min.position.y, min.position.z);
//    glVertex3d(max.position.x, max.position.y, max.position.z);
//    glEnd();

    mesh.render();
}

#endif // !ICO_H
