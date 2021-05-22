#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "../main/graphics.h"
#include "../math/point2d.h"
#include "../math/point3d.h"

#include <array>
#include <vector>

using std::array;
using std::vector;

struct vertex_t {
    vertex_t() = default;
    vertex_t(const point3d& position, const point2d& texture_coord, const point3d& normal)
        : position(position), texture_coord(texture_coord), normal(normal) {}

    point3d position;
    point2d texture_coord;
    point3d normal;
};

struct face_t {
    face_t() = default;
    face_t(const array<vertex_t, 3>& vertices)
        : vertices(vertices) {}

    array<vertex_t, 3> vertices;
};

struct mesh_t {

    void render() const {
        glBegin(GL_TRIANGLES);

        for (const auto& face : faces) {
            for (const auto& vertex : face.vertices) {
                glNormal3d(vertex.normal.x, vertex.normal.y, vertex.normal.z);
                glVertex3d(vertex.position.x, vertex.position.y, vertex.position.z);
            }
        }

        glEnd();
    }

    vector<face_t> faces;
};

#endif // !MESH_COMPONENT_H
