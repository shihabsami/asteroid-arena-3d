#include "game_object.h"

game_object::game_object() = default;

game_object::game_object(const vector3d& position, const vector3d& direction, const vector3d& scale,
    const quaternion& rotation, double velocity)
    : position(), direction(), scale(), rotation(), velocity(), radius() {}
