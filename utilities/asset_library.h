#ifndef ASSET_LIBRARY_H
#define ASSET_LIBRARY_H

#include "../main/graphics.h"
#include "../utilities/object_loader.h"

#define TEX_DIR_PATH (".." PATH_SEPARATOR "resources" PATH_SEPARATOR "textures" PATH_SEPARATOR)
#define MODEL_DIR_PATH (".." PATH_SEPARATOR "resources" PATH_SEPARATOR "models" PATH_SEPARATOR)

#define SKYBOX_FRONT_TEX_KEY "skybox_front.png"
#define SKYBOX_BACK_TEX_KEY "skybox_back.png"
#define SKYBOX_TOP_TEX_KEY "skybox_top.png"
#define SKYBOX_BOTTOM_TEX_KEY "skybox_bottom.png"
#define SKYBOX_LEFT_TEX_KEY "skybox_left.png"
#define SKYBOX_RIGHT_TEX_KEY "skybox_right.png"

#define SPACESHIP_TEX_KEY "spaceship.png"
#define SPACESHIP_BODY_MODEL_KEY "spaceship_body.obj"
#define SPACESHIP_L_WING_MODEL_KEY "spaceship_l_wing.obj"
#define SPACESHIP_R_WING_MODEL_KEY "spaceship_r_wing.obj"

#define ASTEROID_TEX_KEY(i) ("asteroid_" #i ".jpg")
#define ASTEROID_TEX_COUNT 6
#define ASTEROID_MODEL_KEY "asteroid"

#define SPHERE_SECTORS  50
#define SPHERE_STACKS   50

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class asset_library {
public:
    static void load_textures();
    static void load_models();
    static shared_ptr<mesh_t> sphere_model();

    static unordered_map<string, GLuint> textures;
    static unordered_map<string, shared_ptr<mesh_t>> models;
};

#endif // !ASSET_LIBRARY_H
