#ifndef WALL_H
#define WALL_H

#include "../main/config.h"
#include "../utilities/lighting_component.h"
#include "../utilities/mesh_component.h"

class wall {
public:
    wall(mesh_t mesh, const material_t& material);
    void draw() const;

    mesh_t mesh;
    material_t material;
};

#endif // !WALL_H
