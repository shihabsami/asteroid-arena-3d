#ifndef WALL_H
#define WALL_H

#include "../main/config.h"
#include "../math/vector3d.h"
#include "../utilities/mesh_component.h"
#include "../utilities/lighting_component.h"

class wall {
public:
    wall(const vector3d& position, vector<line_t>& lines);
    void current_material(const material_t& m);
    void update();
    void draw() const;

    double t{};
    vector3d position;
    vector<line_t> lines;
    material_t old_material;
    material_t new_material;
};

#endif // !WALL_H
