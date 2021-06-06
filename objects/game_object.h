#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../math/vector3d.h"
#include "../math/quaternion.h"
#include "../utilities/mesh_component.h"

#include <memory>

using std::shared_ptr;

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
        const shared_ptr<mesh_t>& mesh, GLuint texture_id, const vector3d& position, const quaternion& rotation,
        const vector3d& scale, const vector3d& direction, double velocity);

    virtual ~game_object();

    // initialise the game object
    virtual void init() = 0;

    // calculate the current position/direction of the game object
    virtual void movement() = 0;

    // draw the game object
    virtual void draw() = 0;

    // the mesh for the game object as a collection of faces
    shared_ptr<mesh_t> mesh;

    // a texture handle for the game object generated beforehand
    GLuint texture_id{};

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
