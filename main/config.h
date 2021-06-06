#ifndef CONFIG_H
#define CONFIG_H

// initial configurations
#define GAME_TITLE      "Asteroid Arena 3D"
#define INIT_WIDTH      1280
#define INIT_HEIGHT     720
#define IS_FULLSCREEN   true

// the total number of units in the z axis when aspect ratio is 1.0
#define TOTAL_UNITS   2500

// constants used in transformations
#define ORIGIN_X       0
#define ORIGIN_Y       0
#define Z_NEAR         0.1
#define Z_FAR          5 * TOTAL_UNITS + Z_NEAR
#define FOV            45.0

#define CAMERA_LAG        2.5
#define CAMERA_Y_OFFSET   200.0
#define CAMERA_Z_OFFSET   600.0

#define ARENA_WIDTH              TOTAL_UNITS
#define ARENA_HEIGHT             TOTAL_UNITS
#define ARENA_LENGTH             TOTAL_UNITS
#define ARENA_WALL_WIDTH         1.0
#define ARENA_WALL_DIVISIONS     100
#define ARENA_WALL_LERP_FACTOR   1.0

#define SPACESHIP_SCALE               100.0
#define SPACESHIP_VELOCITY            750.0
#define SPACESHIP_ROTATION_DELTA      90.0
#define SPACESHIP_SPEED_UP_RATE       0.5
#define SPACESHIP_WING_MAX_ROTATION   25.0

#define ASTEROID_MIN_ROTATION   45.0
#define ASTEROID_MAX_ROTATION   90.0
#define ASTEROID_MIN_SCALE      45.0
#define ASTEROID_MAX_SCALE      90.0
#define ASTEROID_MIN_VELOCITY   250.0
#define ASTEROID_MAX_VELOCITY   500.0
#define ASTEROID_HP_MODIFIER    45.0

#define WAVE_INTERVAL              1000
#define MAX_ASTEROID_COUNT         20
#define FIRING_INTERVAL            100
#define WALL_COLLISION_THRESHOLD   1000.0

#define BULLET_PARTICLE_SIZE                10.0
#define BULLET_PARTICLE_VELOCITY            1000.0
#define BULLET_PARTICLE_ROTATION_DELTA      90.0
#define EXPLOSION_PARTICLE_SIZE             50.0
#define EXPLOSION_PARTICLE_MIN_VELOCITY     100.0
#define EXPLOSION_PARTICLE_MAX_VELOCITY     250.0
#define EXPLOSION_PARTICLE_ROTATION_DELTA   90.0
#define EXPLOSION_PARTICLE_DECAY_RATE       1.0
#define EXPLOSION_PARTICLE_COUNT            10

#endif // !CONFIG_H
