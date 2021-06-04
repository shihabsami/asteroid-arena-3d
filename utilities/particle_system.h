#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>

class asteroid;
class bullet;
class particle;
class vector2d;
class object_register;

class particle_system {
public:
    particle_system(object_register* registry);

    void init_exhaust_particles();

    void init_explosion_particles(asteroid* a);

    void init_bullet_particles(bullet* b);

    void init_blackhole_particles();

    void init_starfield();

    void update_particle_properties(std::vector<particle*>* particles);

    void update();

    void draw();

    void clear();

    object_register* registry;
    std::vector<particle*> exhaust_particles;
    std::vector<std::vector<particle*>> explosion_particles;
    std::vector<std::vector<particle*>> bullet_particles;
    std::vector<std::vector<particle*>> blackhole_particles;
    std::vector<particle*> stars;
    double l_exhaust_time;
    double l_blackhole_time;
};

#endif // !PARTICLE_SYSTEM_H
