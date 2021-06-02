#ifndef WALL_H
#define WALL_H

#include "../main/config.h"
#include "../math/vector3d.h"
#include "../utilities/mesh_component.h"
#include "../utilities/lighting_component.h"

class wall {
public:
    wall(const vector3d& position, vector<line_t>& lines, const material_t& material);
    void draw() const;

    vector3d position;
    vector<line_t> lines;
    material_t material;
};

#endif // !WALL_H
