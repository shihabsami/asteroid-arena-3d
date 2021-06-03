#ifndef CONFIG_H
#define CONFIG_H

// initial configurations
#define GAME_TITLE      "Asteroid Arena 3D"
#define INIT_WIDTH      1280
#define INIT_HEIGHT     720
#define IS_FULLSCREEN   true

// the total number of units in the z axis when aspect ratio is 1.0
#define TOTAL_UNITS   1000

// constants used in transformations
#define ORIGIN_X       0
#define ORIGIN_Y       0
#define Z_NEAR         0.1
#define Z_FAR          5 * TOTAL_UNITS + Z_NEAR
#define FOV            45.0

#define CAMERA_LAG 5.0

#define ARENA_WIDTH                1000.0
#define ARENA_HEIGHT               1000.0
#define ARENA_LENGTH               1000.0
#define ARENA_WALL_WIDTH           1.0

#define SPACESHIP_SCALE 100.0
#define SPACESHIP_VELOCITY 500.0
#define SPACESHIP_ROTATION_DELTA 90.0

#define ASTEROID_MIN_ROTATION 45.0
#define ASTEROID_MAX_ROTATION 90.0
#define ASTEROID_HP_MODIFIER 10

#endif // !CONFIG_H
