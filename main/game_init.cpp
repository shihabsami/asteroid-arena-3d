#include "game_init.h"
#include "config.h"
#include "game_camera.h"
#include "../math/game_math.h"
#include "../utilities/input_handler.h"
#include "../utilities/mesh_component.h"
#include "../utilities/object_loader.h"
#include "../objects/game_arena.h"
#include "../utilities/object_register.h"
#include "draw_sphere.h"

#define GL_CLAMP_TO_EDGE 0x812F
#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"
#include "draw_sphere.h"

#include <array>
#include <memory>
#include <cmath>

using std::array;
using std::shared_ptr;
using std::make_shared;

shared_ptr<vector3d> forward = make_shared<vector3d>(0.0, 0.0, -1.0);

shared_ptr<game_camera> camera;
shared_ptr<vector3d> cube_position = make_shared<vector3d>(0.0, 50.0, 0.0);
shared_ptr<vector3d> cube_direction = make_shared<vector3d>(0.0, 0.0, -1.0);
shared_ptr<quaternion> rotation = make_shared<quaternion>(quaternion::get_identity());

shared_ptr<game_arena> arena;

double velocity = 500.0;
double d_angle = 90.0;
double l_time;
bool mouse_control = false;


quaternion old_cam;
quaternion new_cam;

shared_ptr<vector3d> x_axis = make_shared<vector3d>(1.0, 0.0, 0.0);
shared_ptr<vector3d> y_axis = make_shared<vector3d>(0.0, 1.0, 0.0);
shared_ptr<vector3d> z_axis = make_shared<vector3d>(0.0, 0.0, 1.0);

GLuint tex_front;
GLuint tex_back;
GLuint tex_top;
GLuint tex_bottom;
GLuint tex_left;
GLuint tex_right;

shared_ptr<mesh_t> ship;
GLuint ship_tex;

shared_ptr<mesh_t> body;
shared_ptr<mesh_t> wing_left;
shared_ptr<mesh_t> wing_right;

void update_axes(const quaternion& q) {
    x_axis->rotate(q);
    y_axis->rotate(q);
    z_axis->rotate(q);
}

void draw_axes() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLineWidth(4.0);
    set_material(material::red);
    glBegin(GL_LINES);
    glVertex3d(-500.0, 0.0, 0.0);
    glVertex3d(500.0, 0.0, 0.0);
    glEnd();

    set_material(material::green);
    glBegin(GL_LINES);
    glVertex3d(0.0, -500.0, 0.0);
    glVertex3d(0.0, 500.0, 0.0);
    glEnd();

    set_material(material::blue);
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -500.0);
    glVertex3d(0.0, 0.0, 500.0);
    glEnd();

    glPopMatrix();
}

void draw_skybox() {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    glPushMatrix();

    // front
    glBindTexture(GL_TEXTURE_2D, tex_front);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(1.0, -1.0, -1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glEnd();

    // back
    glBindTexture(GL_TEXTURE_2D, tex_back);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glEnd();

    // top
    glBindTexture(GL_TEXTURE_2D, tex_top);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(1.0, 1.0, -1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glEnd();

    // bottom
    glBindTexture(GL_TEXTURE_2D, tex_bottom);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(1.0, -1.0, -1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glEnd();

    // left
    glBindTexture(GL_TEXTURE_2D, tex_left);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(-1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(-1.0, -1.0, -1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(-1.0, 1.0, -1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(-1.0, 1.0, 1.0);
    glEnd();

    // right
    glBindTexture(GL_TEXTURE_2D, tex_right);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3d(1.0, -1.0, -1.0);
    glTexCoord2d(1.0, 0.0);
    glVertex3d(1.0, -1.0, 1.0);
    glTexCoord2d(1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glTexCoord2d(0.0, 1.0);
    glVertex3d(1.0, 1.0, -1.0);
    glEnd();

    glPopMatrix();

    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
}

void pitch(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            x_axis->x * sin(to_radians(angle * 0.5)),
            x_axis->y * sin(to_radians(angle * 0.5)),
            x_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    cube_direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
}

void roll(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            z_axis->x * sin(to_radians(angle * 0.5)),
            z_axis->y * sin(to_radians(angle * 0.5)),
            z_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    cube_direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
}

void yaw(bool negative, double amount) {
    double angle = (negative ? -d_angle : d_angle) * amount * g::d_time;
    quaternion qr = quaternion(
        cos(to_radians(angle * 0.5)), {
            y_axis->x * sin(to_radians(angle * 0.5)),
            y_axis->y * sin(to_radians(angle * 0.5)),
            y_axis->z * sin(to_radians(angle * 0.5))
        }
    );

    *rotation = qr * *rotation;
    cube_direction = make_shared<vector3d>(forward->get_rotated(*rotation));
    camera->rotate(*rotation);
    update_axes(qr);
}

double l_rot = 0.0;
double r_rot = 0.0;

void draw_ship() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ship_tex);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScaled(10.0, 10.0, 10.0);
    body->render();

    glPushMatrix();
    glRotated(l_rot, 0.0, 0.0, 1.0);
    wing_left->render();
    glPopMatrix();

    glPushMatrix();
    glRotated(r_rot, 0.0, 0.0, 1.0);
    wing_right->render();
    glPopMatrix();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

GLuint particle_tex;
GLuint moon_tex;
GLuint uv_grid_tex;

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor
        (
            static_cast<GLclampf>(color::black.r),
            static_cast<GLclampf>(color::black.g),
            static_cast<GLclampf>(color::black.b), 1.0
        );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera->orient();
    draw_skybox();
    camera->translate();

    GLfloat light0_position[4]{ -0.8, 0.8, -0.8, 0.0 };
    set_light(GL_LIGHT0, material::light, light0_position);

    draw_axes();
    arena->draw();

    glPushMatrix();
    glTranslated(cube_position->x, cube_position->y, cube_position->z);
    glMultMatrixd(rotation->to_matrix());
    glScaled(10.0, 10.0, 10.0);
    set_material(material::purple);
    draw_ship();
    glPopMatrix();

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, moon_tex);
//    glPushMatrix();
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::white);
//    draw_sphere(1.0, 100, 100);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    glTranslated(100.0, 0.0, 0.0);
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::white);
//    draw_ico(1.0, 5);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    glTranslated(200.0, 0.0, 0.0);
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::purple);
//    draw_ico(1.0, 1);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//
//
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    glTranslated(400.0, 0.0, 0.0);
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::purple);
//    draw_ico(1.0, 2);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//
//
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    glTranslated(600.0, 0.0, 0.0);
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::purple);
//    draw_ico(1.0, 3);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    glTranslated(800.0, 0.0, 0.0);
//    glScaled(100.0, 100.0, 100.0);
//    set_material(material::purple);
//    draw_ico(1.0, 5);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);


//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, uv_grid_tex);
//    glPushMatrix();
//    set_material(material::white);
//    glScaled(50.0, 50.0, 50.0);
//    draw_sphere(1.0, 10, 10);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);

//    glDisable(GL_TEXTURE_2D);
//    glEnable(GL_TEXTURE_2D);
//    glDisable(GL_LIGHTING);
//    glBindTexture(GL_TEXTURE_2D, particle_tex);
//
//    glPushMatrix();
//    glMultMatrixd(camera->orientation.to_matrix());
//    glBegin(GL_QUADS);
//    glTexCoord2d(0.0, 0.0);
//    glVertex3d(-50.0, -50.0, 0.0);
//    glTexCoord2d(1.0, 0.0);
//    glVertex3d(50.0, -50.0, 0.0);
//    glTexCoord2d(1.0, 1.0);
//    glVertex3d(50.0, 50.0, 0.0);
//    glTexCoord2d(0.0, 1.0);
//    glVertex3d(-50.0, 50.0, 0.0);
//    glEnd();
//
//    glPopMatrix();
//    glEnable(GL_LIGHTING);
//    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    set_material(material::white);
    glBegin(GL_LINES);
    glVertex3d(cube_position->x, cube_position->y, cube_position->z);
    vector3d target = *cube_position + *cube_direction * 100.0;
    glVertex3d(target.x, target.y, target.z);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
    error_check("game_init::on_display");
}

void movement() {
    if (input::key_states[Q])
        yaw(false, 1.0);
    if (input::key_states[E])
        yaw(true, 1.0);
    if (input::key_states[W])
        pitch(true, 1.0);
    if (input::key_states[S])
        pitch(false, 1.0);
    if (input::key_states[D]) {
        roll(true, 1.0);
        l_rot -= 0.5;
    }
    if (input::key_states[A]) {
        roll(false, 1.0);
        r_rot += 0.5;
    }
    if (input::key_states[SPACEBAR]) {
        *cube_position += *cube_direction * velocity * g::d_time;
    }

    if (input::key_states[X]) {
        double angle = 180.0;
        quaternion qr = quaternion(
            cos(to_radians(angle * 0.5)), {
                y_axis->x * sin(to_radians(angle * 0.5)),
                y_axis->y * sin(to_radians(angle * 0.5)),
                y_axis->z * sin(to_radians(angle * 0.5))
            }
        );

        camera->rotate(*rotation * qr);
        vector3d target = *cube_position + *cube_direction * 200.0;
        vector3d up_offset = { 0, 1, 0 };
        up_offset.rotate(camera->orientation);
        camera->move(target + up_offset * 25.0);
    } else {
        camera->rotate(*rotation);
        vector3d target = *cube_position - *cube_direction * 200.0;
        vector3d up_offset = { 0, 1, 0 };
        up_offset.rotate(camera->orientation);
        camera->move(target + up_offset * 25.0);
    }

//    mouse_control = input::key_states[X];
//    if (mouse_control) {
//        pitch(input::y_delta < 0.0, abs(input::y_delta));
//        yaw(input::x_delta > 0.0, abs(input::x_delta));
//    }
}

void on_reshape(int width, int height) {
    glViewport(ORIGIN_X, ORIGIN_Y, width, height);
    g::width = static_cast<double>(width);
    g::height = static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    error_check("game_init::on_reshape");
}

void on_idle() {
    // rotate current and delta time
    g::c_time = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
    g::d_time = (g::c_time - l_time) / 1000;
    l_time = g::c_time;

    movement();
    camera->update();
    glutPostRedisplay();
    error_check("game_init::on_idle");
}

// input functions delegate calls to handler class
void on_mouse_click(int button, int state, int x, int y) {
    input_handler::on_mouse_click(button, state, x, y);
    error_check("game_init::on_mouse_click");
}

void on_key_down(unsigned char key, int x, int y) {
    input_handler::on_key_down(key, x, y);
    error_check("game_init::on_key_down");
}

void on_key_up(unsigned char key, int x, int y) {
    input_handler::on_key_up(key, x, y);
    error_check("game_init::on_key_up");
}

void on_mouse_motion(int x, int y) {
    input_handler::on_mouse_motion(x, y);
    error_check("game_init::on_mouse_motion");
}

void init_camera() {
    vector3d camera_position{ 0.0, 50.0, TOTAL_UNITS / 2.0 };
    camera = make_shared<game_camera>(camera_position, quaternion::get_identity());
}

void load_texture(GLuint& textureHandle, const char* filename) {
    int width, height, components;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename, &width, &height, &components, STBI_rgb_alpha);
    glPushAttrib(GL_TEXTURE_BIT);
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
    glPopAttrib();
}

void init_lighting() {
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);

//    GLfloat global_ambient_rgba[] = { 0.2, 0.2, 0.2, 1.0 };
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_rgba);

    GLfloat light0_position[4]{ -0.8, 0.8, -0.8, 0.0 };
    set_light(GL_LIGHT0, material::light, light0_position);
}

void init_textures() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

    load_texture(tex_front,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\front.png)");
    load_texture(tex_back,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\back.png)");
    load_texture(tex_top,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\top.png)");
    load_texture(tex_bottom,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\bottom.png)");
    load_texture(tex_left,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\left.png)");
    load_texture(tex_right,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\skybox\right.png)");

    load_texture(particle_tex,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\fireball.png)");

    load_texture(uv_grid_tex,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\uv.png)");

    load_texture(moon_tex,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\moon.jpg)");

    load_texture(ship_tex,
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\files\models\ship\ship.png)");
}

void init_game(int* argcp, char** argv, game_window* window) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(window->position_x, window->position_y);
    glutInitWindowSize(window->width, window->height);

    g::width = static_cast<double>(window->width);
    g::height = static_cast<double>(window->height);

    glutCreateWindow(GAME_TITLE);
    // if (window->is_fullscreen) glutFullScreen();

    // register callback methods
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutIdleFunc(on_idle);

    glutMouseFunc(on_mouse_click);
    glutPassiveMotionFunc(on_mouse_motion);
    glutIgnoreKeyRepeat(false);
    glutKeyboardFunc(on_key_down);
    glutKeyboardUpFunc(on_key_up);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, g::width / g::height, Z_NEAR, Z_FAR);

    init_camera();
    init_lighting();
    init_textures();

    arena = make_shared<game_arena>(TOTAL_UNITS, TOTAL_UNITS, TOTAL_UNITS);
    std::string a = argv[0];
    a.append(R"(\..\..\files\models\ship\ship.obj)");

    ship = load_obj(
        a.c_str());

    body = load_obj(
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\files\models\ship_hierarchical\body.obj)");
    wing_left = load_obj(
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\files\models\ship_hierarchical\wing_left.obj)");
    wing_right = load_obj(
        R"(C:\Users\Quartzenstein\Desktop\Interactive 3D Graphics and Animation\Asteroid Arena 3D\files\models\ship_hierarchical\wing_right.obj)");

    reg

    glutMainLoop();
}
