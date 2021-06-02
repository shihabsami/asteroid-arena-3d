#include "game_object.h"

#include <utility>

using std::move;

game_object::game_object() = default;

game_object::game_object(
    shared_ptr<mesh_t>& mesh, shared_ptr<vector3d>& position, shared_ptr<quaternion>& rotation,
    shared_ptr<vector3d>& scale, shared_ptr<vector3d>& direction, double velocity)
    : mesh(move(mesh)), position(move(position)), rotation(move(rotation)),
    scale(move(scale)), direction(move(direction)), velocity(velocity) {}

game_object::~game_object() {
    mesh.reset();
    position.reset();
    rotation.reset();
    scale.reset();
    direction.reset();
}
