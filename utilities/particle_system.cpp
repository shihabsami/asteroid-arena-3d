#include "particle_system.h"
#include "object_register.h"

particle_system::particle_system(object_register* registry)
    : registry(registry), l_exhaust_time(), l_blackhole_time() {}

// create particles slightly behind the spaceship
void particle_system::init_exhaust_particles() {
    vector2d position = registry->ship->position - (registry->ship->direction * registry->ship->radius * 1.15);
    exhaust_particles.push_back(
        new particle(position, vector2d(), EXHAUST_PARTICLE_SIZE, 0.0, 0.0, 1.0,
            EXHAUST_PARTICLE_DECAY_RATE, c::exhaust_start, c::exhaust_end));
}

// create explosion particles at the asteroid's location
void particle_system::init_explosion_particles(asteroid* a) {
    std::vector<particle*> particles;
    for (int i = 0; i < EXPLOSION_PARTICLE_COUNT; ++i) {
        double velocity = get_random(20.0, 60.0);
        double angle = get_random(0.0, MAX_ANGLE);
        color_t start_color = lerp(c::explosion_start, c::explosion_end, get_random(0.0, 1.0));
        vector2d direction{ cos(to_radians(angle)), sin(to_radians(angle)) };
        particles.push_back(
            new particle(a->position, direction, EXPLOSION_PARTICLE_SIZE, 0.0, velocity, 1.0,
                EXPLOSION_PARTICLE_DECAY_RATE, start_color, c::explosion_end));
    }

    explosion_particles.push_back(particles);
}

// create particles at the location where the bullet hit
void particle_system::init_bullet_particles(bullet* b) {
    std::vector<particle*> particles;
    double angle = (b->direction * -1).get_angle();
    double angle_modifier = -20.0;
    for (int i = 0; i < BULLET_PARTICLE_COUNT; ++i) {
        double velocity = get_random(40.0, 80.0);
        color_t start_color = lerp(c::bullet_particle_start, c::bullet_particle_end, get_random(0.0, 1.0));
        vector2d direction{ cos(to_radians(angle + angle_modifier)), sin(to_radians(angle + angle_modifier)) };
        angle_modifier += 10.0;
        particles.push_back(
            new particle(b->position, direction, BULLET_PARTICLE_SIZE, 0.1, velocity, 1.0,
                BULLET_PARTICLE_DECAY_RATE, start_color, c::bullet_particle_end));
    }

    bullet_particles.push_back(particles);
}

// create particles around the blackhole
void particle_system::init_blackhole_particles() {
    blackhole* bh = registry->bh;
    std::vector<particle*> particles;
    for (int i = 0; i < bh->points.size(); ++i) {
        vector2d position{
            bh->position.x + bh->points.at(i)->x * bh->size + get_random(-0.1, 0.1),
            bh->position.y + bh->points.at(i)->y * bh->size + get_random(-0.1, 0.1) };
        vector2d normal = (bh->position - position).get_normalized();
        vector2d tangent{ -normal.y, normal.x };
        double velocity = get_random(10.0, 30.0);
        particles.push_back(
            new particle(position, tangent, BLACKHOLE_PARTICLE_SIZE, -0.75, velocity, 1.0,
                BLACKHOLE_PARTICLE_DECAY_RATE, c::black, c::gray));
    }

    blackhole_particles.push_back(particles);
}

void particle_system::init_starfield() {
    for (int i = 0; i < STAR_PARTICLE_COUNT; ++i) {
        vector2d position = {
            get_random(-g::x_units / 2, g::x_units / 2),
            get_random(-g::y_units / 2, g::y_units / 2) };
        vector2d normal = (registry->bh->position - position).get_normalized();
        vector2d tangent{ -normal.y, normal.x };
        double velocity = get_random(5.0, 30.0);
        stars.push_back(
            new particle(position, tangent, get_random(0.1, STAR_PARTICLE_SIZE), -1.0, velocity, 0.0,
                STAR_PARTICLE_DECAY_RATE, c::gray, c::gray));
    }
}

void particle_system::update_particle_properties
    (std::vector<particle*>* particles) {
    for (int i = 0; i < particles->size(); ++i) {
        particle* p = particles->at(i);
        p->update();
        p->movement();
        if (p->t == 0.0) {
            particles->erase(particles->begin() + i);
            delete p;
        }
    }
}

void particle_system::update() {
    update_particle_properties(&exhaust_particles);

    for (int i = 0; i < explosion_particles.size(); ++i) {
        auto particles = &explosion_particles.at(i);
        update_particle_properties(particles);
        if (particles->empty()) explosion_particles.erase(explosion_particles.begin() + i);
    }

    for (int i = 0; i < bullet_particles.size(); ++i) {
        auto particles = &bullet_particles.at(i);
        update_particle_properties(particles);
        if (particles->empty()) bullet_particles.erase(bullet_particles.begin() + i);
    }

    if (!registry->ready()) return;

    for (int i = 0; i < blackhole_particles.size(); ++i) {
        auto particles = &blackhole_particles.at(i);
        bool rotate = false;
        for (particle* p : *particles) {
            vector2d normal = (registry->bh->position - p->position).get_normalized();
            vector2d tangent{ -normal.y, normal.x };
            p->direction = (rotate) ? tangent : (p->direction + (normal / BLACKHOLE_PARTICLE_PULL)).get_normalized();
            rotate = !rotate;
        }

        update_particle_properties(particles);
        if (particles->empty()) blackhole_particles.erase(blackhole_particles.begin() + i);
    }

    for (particle* p : stars) {
        vector2d normal = (registry->bh->position - p->position).get_normalized();
        vector2d tangent{ -normal.y, normal.x };
        p->direction = tangent;
        p->update();
        p->movement();
        if (p->t == 0.0) p->lerp_ascending = true;
        else if (p->t == 1.0) p->lerp_ascending = false;
    }
}

void particle_system::draw() {
    if (g::c_time - l_exhaust_time >= EXHAUST_PARTICLE_INTERVAL
        && registry->ship->is_moving) {
        l_exhaust_time = g::c_time;
        init_exhaust_particles();
    }

    if (g::c_time - l_blackhole_time >= BLACKHOLE_PARTICLE_INTERVAL) {
        l_blackhole_time = g::c_time;
        init_blackhole_particles();
    }

    if (stars.empty()) init_starfield();

    for (particle* p : exhaust_particles)
        p->draw();

    for (auto ps : explosion_particles)
        for (particle* p : ps)
            p->draw();

    for (auto ps : bullet_particles)
        for (particle* p : ps)
            p->draw();

    for (auto ps : blackhole_particles)
        for (particle* p : ps)
            p->draw();

    for (particle* p : stars)
        p->draw();

    error_check("particle_system::draw");
}

void particle_system::clear() {
    for (particle* p : exhaust_particles)
        delete p;

    for (auto ps : explosion_particles)
        for (particle* p : ps)
            delete p;

    for (auto ps : bullet_particles)
        for (particle* p : ps)
            delete p;

    for (auto ps : blackhole_particles)
        for (particle* p : ps)
            delete p;

    for (particle* p : stars)
        delete p;

    exhaust_particles.clear();
    explosion_particles.clear();
    bullet_particles.clear();
    blackhole_particles.clear();
    stars.clear();
}
