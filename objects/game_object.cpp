#include "game_object.h"

game_object::game_object() = default;

game_object::game_object(
    const shared_ptr<mesh_t>& mesh, const vector3d& position, const quaternion& rotation,
    const vector3d& scale, const vector3d& direction, double velocity)
    : mesh(mesh), position(position), rotation(rotation),
    scale(scale), direction(direction), velocity(velocity) {}

game_object::~game_object() {
    mesh.reset();
}
