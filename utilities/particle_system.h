#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../objects/particle.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

class spaceship;
class asteroid;
class game_camera;

class particle_system {
public:
    void add_bullet(const shared_ptr<spaceship>& s);

    void add_explosion(const shared_ptr<asteroid>& a);

    void update(const shared_ptr<game_camera>& camera);

    void draw_particles();

    void clear();

    vector<shared_ptr<particle>> bullet_particles;
    vector<vector<shared_ptr<particle>>> explosion_particles;
};

#endif // !PARTICLE_SYSTEM_H
