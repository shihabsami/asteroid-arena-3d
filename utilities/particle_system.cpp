#include "particle_system.h"
#include "asset_library.h"
#include "../main/config.h"
#include "../main/game_camera.h"
#include "../objects/spaceship.h"
#include "../objects/asteroid.h"

using std::sort;

void particle_system::add_bullet(const shared_ptr<spaceship>& s) {
    shared_ptr<particle> p = make_shared<particle>(
        asset_library::models.at(PARTICLE_MODEL_KEY),
        asset_library::textures.at(BULLET_TEX_KEY),
        s->position + s->direction * s->radius / 2, quaternion{},
        vector3d{ BULLET_PARTICLE_SIZE, BULLET_PARTICLE_SIZE, BULLET_PARTICLE_SIZE },
        s->direction, s->velocity + BULLET_PARTICLE_VELOCITY, BULLET_PARTICLE_ROTATION_DELTA,
        0.0, 6, 6
    );

    p->init();
    bullet_particles.push_back(p);
}

void particle_system::add_explosion(const shared_ptr<asteroid>& a) {
    vector<shared_ptr<particle>> particles;
    for (int i = 0; i < EXPLOSION_PARTICLE_COUNT; ++i) {
        double theta = get_random(0.0, M_PI * 2);
        double phi = acos((2 * get_random(0.0, 1.0)) - 1);
        vector3d position = vector3d{
            a->radius * sin(phi) * cos(theta),
            a->radius * sin(phi) * sin(theta),
            a->radius * cos(phi)
        } + a->position;

        vector3d direction = (position - a->position).get_normalised();
        double velocity = get_random(EXPLOSION_PARTICLE_MIN_VELOCITY, EXPLOSION_PARTICLE_MAX_VELOCITY);

        shared_ptr<particle> p = make_shared<particle>(
            asset_library::models.at(PARTICLE_MODEL_KEY),
            asset_library::textures.at(EXPLOSION_TEX_KEY),
            position, quaternion{},
            vector3d{ EXPLOSION_PARTICLE_SIZE, EXPLOSION_PARTICLE_SIZE, EXPLOSION_PARTICLE_SIZE },
            direction, velocity, EXPLOSION_PARTICLE_ROTATION_DELTA, EXPLOSION_PARTICLE_DECAY_RATE,
            8, 6
        );

        p->init();
        particles.push_back(p);
    }

    explosion_particles.push_back(particles);
}

void particle_system::update(const shared_ptr<game_camera>& camera) {
    for (const auto& b : bullet_particles) {
        b->orientation = camera->orientation;
        b->movement();
    }

    for (int i = (int) explosion_particles.size() - 1; !explosion_particles.empty() && i >= 0; --i) {
        auto es = explosion_particles.at(i);
        for (int j = (int) es.size() - 1; !es.empty() && j >= 0; --j) {
            auto& e = es.at(j);
            e->orientation = camera->orientation;
            e->update();
            e->movement();
            if (e->t == 0.0)
                es.erase(es.begin() + j);

            if (j == 0) break;
        }

        if (es.empty())
            explosion_particles.erase(explosion_particles.begin() + i);

        if (i == 0) break;
    }

    sort(bullet_particles.begin(), bullet_particles.end(),
        [&](const shared_ptr<particle>& b1, const shared_ptr<particle>& b2) {
            return b1->position.get_distance_squared(camera->position) >
                b2->position.get_distance_squared(camera->position);
        }
    );

    for (auto& es : explosion_particles)
        sort(es.begin(), es.end(),
            [&](const shared_ptr<particle>& e1, const shared_ptr<particle>& e2) {
                return e1->position.get_distance_squared(camera->position) >
                    e2->position.get_distance_squared(camera->position);
            }
        );
}

void particle_system::draw_particles() {
    for (const auto& b : bullet_particles)
        b->draw();

    for (const auto& es : explosion_particles)
        for (const auto& e : es)
            e->draw();

    error_check("particle_system::draw_particles");
}

void particle_system::clear() {
    bullet_particles.clear();
    explosion_particles.clear();
}
