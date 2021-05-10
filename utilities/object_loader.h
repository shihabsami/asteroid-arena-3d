#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "../math/point3d.h"
#include "../math/point3d.h"
#include "mesh_component.h"

using std::string;
using std::stringstream;
using std::ifstream;
using std::vector;
using std::getline;

static vector<vertex> load_obj(const char* file_name) {
    vector<point3d> positions;
    vector<point2d> texture_coords;
    vector<point3d> normals;

    vector<int> position_indices;
    vector<int> texture_coord_indices;
    vector<int> normal_indices;

    vector<vertex> vertices;

    string line;
    stringstream ss;
    ifstream in_file{ file_name };

    if (!in_file.is_open()) throw "not found";

    while (getline(in_file, line)) {
        std::string prefix;

        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v") {
            point3d position;
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (prefix == "vt") {
            point2d tex_coord;
            ss >> tex_coord.x >> tex_coord.y;
            texture_coords.push_back(tex_coord);
        } else if (prefix == "vn") {
            point3d normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "f") {
            int position = 0;
            int index;
            while (ss >> index) {
                if (position == 0)
                    position_indices.push_back(index);
                else if (position == 1)
                    texture_coord_indices.push_back(index);
                else if (position == 2)
                    normal_indices.push_back(index);

                if (ss.peek() == '/') {
                    ss.ignore(1, '/');
                    ++position;
                } else if (ss.peek() == ' ') {
                    ss.ignore(1, ' ');
                    position = 0;
                }
            }
        }
    }

    vertices.resize(position_indices.size(), vertex());
    for (int i = 0; i < vertices.size(); ++i) {
        vertices.at(i).position = positions.at(position_indices.at(i) - 1);
        vertices.at(i).texture_coord = texture_coords.at(texture_coord_indices.at(i) - 1);
        vertices.at(i).normal = normals.at(normal_indices.at(i) - 1);
    }

    return vertices;
}

#endif // !OBJECT_LOADER_H
