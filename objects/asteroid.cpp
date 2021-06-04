#include "asteroid.h"
#include "../main/config.h"
#include "../main/global.h"

void asteroid::init() {
    radius = sqrt(scale.x / 2 * scale.x / 2 + scale.y / 2 * scale.y / 2 + scale.z / 2 * scale.z / 2);
    d_angle = get_random(ASTEROID_MIN_ROTATION, ASTEROID_MAX_ROTATION);
    rotation_axis = {
        get_random(0.0, 1.0),
        get_random(0.0, 1.0),
        get_random(0.0, 1.0)
    };

    hp = static_cast<int>(radius / ASTEROID_HP_MODIFIER) + 1;
    hit_count = 0;
    can_bounce = false;
    can_split = true;
}

void asteroid::movement() {
    rotation = rotation * quaternion{ d_angle * global::d_time, rotation_axis };
    position = position + (direction * velocity * global::d_time);
}

void asteroid::draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.to_matrix());
    glScaled(scale.x, scale.y, scale.z);
    mesh->render();
    glPopMatrix();
    error_check("asteroid::draw");
}
