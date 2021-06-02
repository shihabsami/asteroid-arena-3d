#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include "mesh_component.h"

#include <string>
#include <sstream>
#include <fstream>
#include <memory>

using std::string;
using std::stringstream;
using std::ifstream;
using std::ios_base;
using std::getline;

using std::shared_ptr;
using std::make_shared;
using std::move;

static shared_ptr<mesh_t> load_obj(const char* file_name) {
    shared_ptr<vector<point3d>> positions = make_shared<vector<point3d>>();
    shared_ptr<vector<point2d>> texture_coords = make_shared<vector<point2d>>();
    shared_ptr<vector<point3d>> normals = make_shared<vector<point3d>>();

    shared_ptr<vector<size_t>> position_indices = make_shared<vector<size_t>>();
    shared_ptr<vector<size_t>> texture_coord_indices = make_shared<vector<size_t>>();
    shared_ptr<vector<size_t>> normal_indices = make_shared<vector<size_t>>();

    string line;
    stringstream ss;
    ifstream in_file{ file_name };
    if (!in_file.is_open()) throw ios_base::failure("cannot open file at object_loader::load_obj");

    while (getline(in_file, line)) {
        string prefix;

        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v") {
            point3d position;
            ss >> position.x >> position.y >> position.z;
            positions->push_back(position);
        } else if (prefix == "vt") {
            point2d tex_coord;
            ss >> tex_coord.x >> tex_coord.y;
            texture_coords->push_back(tex_coord);
        } else if (prefix == "vn") {
            point3d normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals->push_back(normal);
        } else if (prefix == "f") {
            int position = 0;
            size_t index = 0;
            while (ss >> index) {
                if (position == 0)
                    position_indices->push_back(index);
                else if (position == 1)
                    texture_coord_indices->push_back(index);
                else if (position == 2)
                    normal_indices->push_back(index);

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

    shared_ptr<mesh_t> mesh = make_shared<mesh_t>();
    mesh->faces.resize(position_indices->size() / 3, face_t{});

    for (size_t i = 0, j = 0; i < position_indices->size(); i += 3, ++j) {
        for (size_t k = 0; k < 3; ++k) {
            mesh->faces.at(j).vertices.at(k).position = positions->at(position_indices->at(i + k) - 1);
            mesh->faces.at(j).vertices.at(k).texcoord = texture_coords->at(texture_coord_indices->at(i + k) - 1);
            mesh->faces.at(j).vertices.at(k).normal = normals->at(normal_indices->at(i + k) - 1);
        }
    }

    return mesh;
}

#endif // !OBJECT_LOADER_H
