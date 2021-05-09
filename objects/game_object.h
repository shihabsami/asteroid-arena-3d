#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../math/vector3d.h"
#include "../math/quaternion.h"

#include <memory>

using std::array;
using std::shared_ptr;
using std::make_shared;

/**
 * An abstract class for game objects to inherit from
 * methods except the constructors are made pure virtual
 * to ensure they are defined by the subclasses.
 */
class game_object {
public:
    // default constructor for constructing default game object
    game_object();

    // construct game object with specified parameters
    game_object(
        const vector3d& position, const vector3d& direction,
        const vector3d& scale, const quaternion& rotation, double velocity);

    // initialise the game object
    virtual void init() = 0;

    // calculate the current position/direction of the game object
    virtual void movement() = 0;

    // draw the game object
    virtual void draw() = 0;

    // position of the game object
    vector3d position;

    // direction of the game object as an unit vector
    vector3d direction;

    // scale of the game object
    vector3d scale;

    // rotation of the game object as an unit quaternion
    quaternion rotation;

    // v of the game object in world coordinates
    double velocity{};

    // the radius that encircles the game object
    double radius{};
};

#endif // !GAME_OBJECT_H
