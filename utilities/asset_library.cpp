#include "asset_library.h"
#include "texture_loader.h"

unordered_map<string, GLuint> asset_library::textures;

unordered_map<string, shared_ptr<mesh_t>> asset_library::models;

void asset_library::load_textures() {
    string tex_dir_path(TEX_DIR_PATH);

    GLuint skybox_front_id;
    load_texture(skybox_front_id, (tex_dir_path + SKYBOX_FRONT_TEX_KEY).c_str());
    textures.insert({ SKYBOX_FRONT_TEX_KEY, skybox_front_id });

    GLuint skybox_back_id;
    load_texture(skybox_back_id, (tex_dir_path + SKYBOX_BACK_TEX_KEY).c_str());
    textures.insert({ SKYBOX_BACK_TEX_KEY, skybox_back_id });

    GLuint skybox_top_id;
    load_texture(skybox_top_id, (tex_dir_path + SKYBOX_TOP_TEX_KEY).c_str());
    textures.insert({ SKYBOX_TOP_TEX_KEY, skybox_top_id });

    GLuint skybox_bottom_id;
    load_texture(skybox_bottom_id, (tex_dir_path + SKYBOX_BOTTOM_TEX_KEY).c_str());
    textures.insert({ SKYBOX_BOTTOM_TEX_KEY, skybox_bottom_id });

    GLuint skybox_left_id;
    load_texture(skybox_left_id, (tex_dir_path + SKYBOX_LEFT_TEX_KEY).c_str());
    textures.insert({ SKYBOX_LEFT_TEX_KEY, skybox_left_id });

    GLuint skybox_right_id;
    load_texture(skybox_right_id, (tex_dir_path + SKYBOX_RIGHT_TEX_KEY).c_str());
    textures.insert({ SKYBOX_RIGHT_TEX_KEY, skybox_right_id });

    GLuint spaceship_id;
    load_texture(spaceship_id, (tex_dir_path + SPACESHIP_TEX_KEY).c_str());
    textures.insert({ SPACESHIP_TEX_KEY, spaceship_id });

    GLuint asteroid_id;
    load_texture(asteroid_id, (tex_dir_path + ASTEROID_TEX_KEY).c_str());
    textures.insert({ ASTEROID_TEX_KEY, asteroid_id });
}

void asset_library::load_models() {
    string model_dir_path(MODEL_DIR_PATH);

    models.insert({ SPACESHIP_BODY_MODEL_KEY,
        load_obj((model_dir_path + SPACESHIP_BODY_MODEL_KEY).c_str()) });
    models.insert({ SPACESHIP_L_WING_MODEL_KEY,
        load_obj((model_dir_path + SPACESHIP_L_WING_MODEL_KEY).c_str()) });
    models.insert({ SPACESHIP_R_WING_MODEL_KEY,
        load_obj((model_dir_path + SPACESHIP_R_WING_MODEL_KEY).c_str()) });

    models.insert({ ASTEROID_MODEL_KEY, sphere_model() });
}

shared_ptr<mesh_t> asset_library::sphere_model() {
    vector<vertex_t> vertices;
    vector<face_t> faces;

    double sector_step = 2 * M_PI / SPHERE_SECTORS;
    double stack_step = M_PI / SPHERE_STACKS;
    double sector_angle, stack_angle;

    for (int i = 0; i <= SPHERE_STACKS; ++i) {
        stack_angle = M_PI / 2 - i * stack_step;
        double xy = cos(stack_angle);
        double z = sin(stack_angle);
        for (int j = 0; j <= SPHERE_SECTORS; ++j) {
            sector_angle = j * sector_step;
            point3d position{ xy * cos(sector_angle), xy * sin(sector_angle), z };
            point2d texcoord{ static_cast<double>(j) / SPHERE_SECTORS, static_cast<double>(i) / SPHERE_STACKS };
            vertices.emplace_back(position, texcoord, position);
        }
    }

    for (int i = 0; i < SPHERE_STACKS; ++i) {
        int k1 = i * (SPHERE_SECTORS + 1);
        int k2 = k1 + SPHERE_SECTORS + 1;

        for (int j = 0; j < SPHERE_SECTORS; ++j, ++k1, ++k2) {
            if (i != 0) {
                faces.emplace_back(array<vertex_t, 3>{
                    vertices.at(k1),
                    vertices.at(k2),
                    vertices.at(k1 + 1)
                });
            }

            if (i != (SPHERE_STACKS - 1)) {
                faces.emplace_back(array<vertex_t, 3>{
                    vertices.at(k1 + 1),
                    vertices.at(k2),
                    vertices.at(k2 + 1)
                });
            }
        }
    }

    return make_shared<mesh_t>(faces);
}
