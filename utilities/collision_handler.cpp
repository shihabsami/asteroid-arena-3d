#include "collision_handler.h"
#include "game_manager.h"
#include "object_register.h"

using std::exception;

collision_handler::collision_handler(const shared_ptr<game_manager>& manager)
    : manager(manager), o_register(manager->o_register) {}

void collision_handler::run() {
    try {
        ship_against_wall();
        // ship_against_asteroid();
        asteroid_against_wall();
        bullet_against_wall();
        bullet_against_asteroid();
        asteroid_against_asteroid();
    } catch (const exception& e) {
        printf("%s\n", e.what());
    }

    error_check("collision_handler::update");
}

void collision_handler::ship_against_wall() {
    if (!o_register->is_ready) return;
    auto ship = o_register->ship;
    auto arena = o_register->arena;

    // update arena wall material if spaceship is close
    arena->walls[FRONT]->current_material(
        ship->position.z - ship->radius - WALL_COLLISION_THRESHOLD <= -arena->length / 2
            ? material::wall_red : material::wall_white);
    arena->walls[BACK]->current_material(
        ship->position.z + ship->radius + WALL_COLLISION_THRESHOLD >= arena->length / 2
            ? material::wall_red : material::wall_white);
    arena->walls[TOP]->current_material(
        ship->position.y + ship->radius + WALL_COLLISION_THRESHOLD >= arena->height / 2
            ? material::wall_red : material::wall_white);
    arena->walls[BOTTOM]->current_material(
        ship->position.y - ship->radius - WALL_COLLISION_THRESHOLD <= -arena->height / 2
            ? material::wall_red : material::wall_white);
    arena->walls[LEFT]->current_material(
        ship->position.x - ship->radius - WALL_COLLISION_THRESHOLD <= -arena->width / 2
            ? material::wall_red : material::wall_white);
    arena->walls[RIGHT]->current_material(
        ship->position.x + ship->radius + WALL_COLLISION_THRESHOLD >= arena->width / 2
            ? material::wall_red : material::wall_white);

    // end game if spaceship collides with arena
    if (ship->position.x - ship->radius <= -arena->width / 2 || ship->position.x + ship->radius >= arena->width / 2 ||
        ship->position.y - ship->radius <= -arena->height / 2 || ship->position.y + ship->radius >= arena->height / 2 ||
        ship->position.z - ship->radius <= -arena->length / 2 || ship->position.z + ship->radius >= arena->length / 2)
        manager->end_game();
}

void collision_handler::ship_against_asteroid() {
    if (!o_register->is_ready) return;
    auto ship = o_register->ship;

    for (const auto& a : o_register->asteroids) {
        double distance_sqr = ship->position.get_distance_squared(a->position);
        double sum_of_radius_sqr = (ship->radius + a->radius) * (ship->radius + a->radius);

        // end game if ship collides with an asteroid
        if (distance_sqr <= sum_of_radius_sqr)
            manager->end_game();
    }
}

void collision_handler::asteroid_against_wall() {
    if (!o_register->is_ready) return;
    auto arena = o_register->arena;

    for (const auto& a : o_register->asteroids) {
        // only allow bouncing once a is fully inside the arena
        if (a->position.x - a->radius > -arena->width / 2 && a->position.x + a->radius < arena->width / 2 &&
            a->position.y - a->radius > -arena->height / 2 && a->position.y + a->radius < arena->height / 2 &&
            a->position.z - a->radius > -arena->length / 2 && a->position.z + a->radius < arena->length / 2)
            a->can_bounce = true;

        // if not within arena, do not check for collision
        if (!a->can_bounce) continue;

        // if a collides with the arena walls
        if (a->position.y + a->radius >= arena->height / 2) {
            // move a away to prevent multiple collisions
            vector3d normal{ 0.0, -1.0, 0.0 };
            double move_distance = (a->position.y + a->radius) - arena->height / 2;
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }

        if (a->position.y - a->radius <= -arena->height / 2) {
            vector3d normal{ 0.0, 1.0, 0.0 };
            double move_distance = -arena->height / 2 - (a->position.y - a->radius);
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }

        if (a->position.x - a->radius <= -arena->width / 2) {
            vector3d normal{ 1.0, 0.0, 0.0 };
            double move_distance = -arena->width / 2 - (a->position.x - a->radius);
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }

        if (a->position.x + a->radius >= arena->width / 2) {
            vector3d normal{ -1.0, 0.0, 0.0 };
            double move_distance = (a->position.x + a->radius) - arena->width / 2;
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }

        if (a->position.z - a->radius <= -arena->length / 2) {
            vector3d normal{ 0.0, 0.0, 1.0 };
            double move_distance = -arena->length / 2 - (a->position.z - a->radius);
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }

        if (a->position.z + a->radius >= arena->length / 2) {
            vector3d normal{ 0.0, 0.0, -1.0 };
            double move_distance = (a->position.z + a->radius) - arena->length / 2;
            a->position += (normal * move_distance);
            a->direction = (a->direction + normal * 2).get_normalised();
        }
    }
}

void collision_handler::bullet_against_wall() {
    if (o_register->is_ready && !o_register->p_system->bullet_particles.empty()) {
        auto arena = o_register->arena;

        for (int i = (int) o_register->p_system->bullet_particles.size() - 1; i >= 0; --i) {
            auto b = o_register->p_system->bullet_particles.at(i);

            // if bullet collides with arena
            if (b->position.x - b->radius <= -arena->width / 2 || b->position.x + b->radius >= arena->width / 2||
                b->position.y - b->radius <= -arena->height / 2 || b->position.y + b->radius >= arena->height / 2 ||
                b->position.z - b->radius <= -arena->length / 2 || b->position.z + b->radius >= arena->length / 2)
                o_register->delete_bullet(i);

            if (i == 0) break;
        }
    }
}

void collision_handler::bullet_against_asteroid() {
    if (o_register->is_ready && !o_register->p_system->bullet_particles.empty() && !o_register->asteroids.empty()) {
        auto bullets = o_register->p_system->bullet_particles;
        for (int i = (int) bullets.size() - 1; i >= 0; --i) {
            auto b = bullets.at(i);
            for (int j = (int) o_register->asteroids.size() - 1; j >= 0; --j) {
                auto a = o_register->asteroids.at(j);

                // if bullet collides with asteroid
                double distance_sqr = a->position.get_distance_squared(b->position);
                double radius_sqr = a->radius * a->radius;
                if (distance_sqr <= radius_sqr) {
                    o_register->delete_bullet(i);
                    if (++a->hit_count == a->hp)
                        o_register->delete_asteroid(j);
                }

                if (j == 0) break;
            }

            if (i == 0) break;
        }
    }
}

void collision_handler::asteroid_against_asteroid() {
    if (o_register->is_ready && !o_register->asteroids.empty()) {

        for (int i = (int) o_register->asteroids.size() - 1; i >= 0; --i) {
            auto a1 = o_register->asteroids.at(i);
            if (!a1->can_bounce) continue;

            for (int j = (int) o_register->asteroids.size() - 1; j >= 0; --j) {
                if (j == i) continue;
                auto a2 = o_register->asteroids.at(j);
                if (!a2->can_bounce) continue;

                double distance_sqr = a1->position.get_distance_squared(a2->position);
                double sum_of_radius_sqr = (a1->radius + a2->radius) * (a1->radius + a2->radius);
                if (distance_sqr <= sum_of_radius_sqr) {
                    vector3d a1_normal = vector3d(a2->position - a1->position).get_normalised();
                    vector3d a2_normal = vector3d(a1->position - a2->position).get_normalised();

                    // move asteroids apart to prevent multiple collisions
                    double move_distance = sqrt(sum_of_radius_sqr) - sqrt(distance_sqr);
                    a1->position -= (a1_normal * move_distance / 2);
                    a2->position -= (a2_normal * move_distance / 2);

                    a1->direction = (a1->direction - a1_normal * 2).get_normalised();
                    a2->direction = (a2->direction - a2_normal * 2).get_normalised();
                }
            }
        }
    }
}
