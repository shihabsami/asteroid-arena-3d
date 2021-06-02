#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../math/vector3d.h"
#include "../math/quaternion.h"
#include "../utilities/mesh_component.h"

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
        shared_ptr<mesh_t>& mesh, shared_ptr<vector3d>& position, shared_ptr<quaternion>& rotation,
        shared_ptr<vector3d>& scale, shared_ptr<vector3d>& direction, double velocity);

    ~game_object();

    // initialise the game object
    virtual void init() = 0;

    // calculate the current position/direction of the game object
    virtual void movement() = 0;

    // draw the game object
    virtual void draw() = 0;

    shared_ptr<mesh_t> mesh;

    // position of the game object
    shared_ptr<vector3d> position;

    // direction of the game object as an unit vector
    shared_ptr<vector3d> direction;

    // scale of the game object
    shared_ptr<vector3d> scale;

    // rotation of the game object as an unit quaternion
    shared_ptr<quaternion> rotation;

    // v of the game object in world coordinates
    double velocity{};

    // the radius that encircles the game object
    double radius{};
};

#endif // !GAME_OBJECT_H
