#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "lighting_component.h"

#include <array>
#include <vector>

using std::array;
using std::vector;

class line_t {
public:
    line_t() = default;

    line_t(const point3d& start, const point3d& end)
        : start(start), end(end) {}

    point3d start;
    point3d end;
};

class vertex_t {
public:
    vertex_t() = default;

    vertex_t(const point3d& position, const point2d& texcoord, const point3d& normal)
        : position(position), texcoord(texcoord), normal(normal) {}

    point3d position;
    point2d texcoord;
    point3d normal;
};

class face_t {
public:
    face_t() = default;

    face_t(const array<vertex_t, 3>& vertices) : vertices(vertices) {}

    array<vertex_t, 3> vertices;
};

class mesh_t {
public:
    mesh_t() = default;

    mesh_t(const vector<face_t>& faces) : faces(faces) {}

    void render() const {
        set_material(material);
        glBegin(GL_TRIANGLES);
        for (const auto& face : faces) {
            for (const auto& vertex : face.vertices) {
                glNormal3d(vertex.normal.x, vertex.normal.y, vertex.normal.z);
                glTexCoord2d(vertex.texcoord.x, vertex.texcoord.y);
                glVertex3d(vertex.position.x, vertex.position.y, vertex.position.z);
            }
        }
        glEnd();
    }

    vector<face_t> faces;
    material_t material{};
};


#endif // !MESH_COMPONENT_H
